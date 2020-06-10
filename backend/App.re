open ReWeb;
open Types;

let maxAge = 30 * 24 * 60 * 60;

let todoItems: ref(TodoItemCollection.t) = ref(
{
    TodoItemCollection.items: [
      {id: 1, title: "Learn ReasonML", sub_text: "And other stuff", complete: true},
      {id: 2, title: "Share types between codebases!", sub_text: "(frontend and backend)", complete: false},
    ]
});

// XXX: Development only

let ofStatus = (status: Response.status, _) =>
  status
  |> Response.of_status(~headers=[("Access-Control-Allow-Origin", "*"),
                                  ("Access-Control-Allow-Methods", "GET, PUT, POST, DELETE"),
                                  ("Access-Control-Allow-Headers", "Content-Type"),
    ])
  |> Lwt.return;

let todoOfId = (id: string): option(TodoItem.t) => {
  switch (id |> int_of_string_opt) {
    | Some(id) => todoItems^.items |> List.find_opt((item: TodoItem.t) => item.id == id)
    | None => None
  }
};

let responseOfJson = (input: Yojson.Safe.t) => {
  input
  |> Yojson.Safe.to_string
  // XXX: Development only!
  |> Response.of_text(~headers=[("access-control-allow-origin", "*")])
  |> Lwt.return;
};

let getTodos = (_request) => {
    todoItems^
    |> TodoItemCollection.to_yojson
    |> responseOfJson;
};

let getTodoById = (id, request) => {
  switch (todoOfId(id)) {
    | Some(item) => item
        |> TodoItem.to_yojson
        |> responseOfJson
    | None => ofStatus(`Not_found: Response.status, request)
  };
};

let createTodo = (id, request) => {
  print_endline("\nCreating Item with id " ++ id);
  let newItem = Request.context(request);

  switch (todoOfId(id)) {
  | Some(_) => ofStatus(`Conflict, request)
  | None => {
      todoItems := {
        TodoItemCollection.items: [newItem, ...todoItems^.items]
      };

      ofStatus(`Created, request);
    }
  };
};

let mutateTodoById = (id, request) => {
  print_endline("\nUpdating Item with id " ++ id);
  let newItem = Request.context(request);

  switch (todoOfId(id)) {
    | Some(foundItem) => {
        let newCollection = { TodoItemCollection.items: [
          newItem,
          ...(todoItems^.items |> List.filter((item: TodoItem.t) => item.id != foundItem.id))
        ]};

        todoItems := newCollection;

        newCollection
        |> TodoItemCollection.to_yojson
        |> responseOfJson
      }
    | None => ofStatus(`Not_found, request);
  };
};

let deleteTodoById = (id, request) => {
  switch (todoOfId(id)) {
  | Some(foundItem) => {
      todoItems := {
        items: todoItems^.items |> List.filter((item: TodoItem.t) => item.id != foundItem.id)
      }
      ofStatus(`No_content, request);
    }
  | None => ofStatus(`Not_found, request)
  }
};

let todosRouter = (route) => {
  switch (route) {
  | (`GET,  [])   => getTodos
  | (`GET,  [""]) => getTodos
  | (`GET,  [id]) => getTodoById(id)
  | (`POST, [id]) => Filter.body_json @@ Filter.body_json_decode(TodoItem.of_yojson) @@ createTodo(id)
  | (`PUT,  [id]) => Filter.body_json @@ Filter.body_json_decode(TodoItem.of_yojson) @@ mutateTodoById(id)
  | (`DELETE, [id]) => deleteTodoById(id)
  | _ => ofStatus(`Not_found);
  };
};

let router = (route) => {
  switch (route) {
  | (`OPTIONS, _) => ofStatus(`No_content)
  | (method,   ["todos", ...path]) => todosRouter @@ (method, path)
  | _ => ofStatus(`Not_found);
  };
};

let () = Server.serve(router, ~port=1235);
