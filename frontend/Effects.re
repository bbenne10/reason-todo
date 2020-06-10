open Types;

let baseApiUri = "http://localhost:1235";

// Optimize for pipefirst because JS
let join = (input: list(string), delimiter: string): string => {
  List.fold_left((acc, input) => acc ++ delimiter ++ input, List.hd(input), List.tl(input))
};

let resolveUri = (path: string): string => [baseApiUri, path] -> join("/");

let createItem = (title: string, subText: string) => {
  open Js.Promise;
  let dispatch = Api.store.dispatch;
  let getState = Api.store.getState;
  let state = getState();

  let highestId = state.items |> List.fold_left((mx: int, item: Types.TodoItem.t) => item.id > mx ? item.id : mx, 0);
  let nextId = highestId + 1;
  let uri = resolveUri("todos/" ++ string_of_int(nextId));
  let newItem = {
    TodoItem.id: nextId,
    title,
    sub_text: subText,  // TODO: Fix this case differential
    complete: false
  };
  let requestInit = Fetch.RequestInit.make(
    ~method_=Post,
    ~body=Fetch.BodyInit.make(newItem |> TodoItem.t_encode |> Js.Json.stringify),
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    ()
  );
  Fetch.fetchWithInit(uri, requestInit)
  |> then_(response => {
      switch (response |> Fetch.Response.ok) {
      | true => {
          dispatch(Api.AddItem(newItem));
          resolve(Some(true));
      }
      | false => {
          // TODO: Alert on failure
          Js.log("Failed?");
          resolve(None);
      }
      };
  })
  |> ignore;
};

let toggleItem = (id: int) => {
  open Js.Promise;

  let dispatch = Api.store.dispatch;
  let getState = Api.store.getState;
  let state = getState();
  let curItem = state.items |> List.find_opt((item: TodoItem.t) => item.id == id);

  switch (curItem) {
  | Some(item) => {
      Js.log("Toggling item with id " ++ string_of_int(id));
      let uri = resolveUri("todos/" ++ string_of_int(id));
      let newItem = {
        ...item,
        complete: !item.complete
      };

      let requestInit = Fetch.RequestInit.make(
        ~method_=Put,
        ~body=Fetch.BodyInit.make(newItem |> TodoItem.t_encode |> Js.Json.stringify),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        ()
      );
      Fetch.fetchWithInit(uri, requestInit)
      |> then_(response => {
          switch (response |> Fetch.Response.ok) {
          | true => {
              dispatch(Api.ToggleItem(id));
              resolve(Some(true));
            }
          | false => {
              // TODO: Alert on failure
              Js.log("Failed?");
              resolve(None);
            }
          };
        })
      |> ignore;
    }
  | None => ();
  };
  ();
};

let deleteItem = (id: int) => {
  let uri = resolveUri("todos/" ++ string_of_int(id));
  let requestInit = Fetch.RequestInit.make(
    ~method_=Delete,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
  ());
  Js.Promise.(
    Fetch.fetchWithInit(uri, requestInit)
    |> then_((resp) => {
        switch (resp |> Fetch.Response.ok) {
        | true => {
          resolve(Api.store.dispatch(Api.DeleteItem(id)))
        }
        | false => {
          resolve();
        }}
      })
    |> ignore
  );
};

let fetchTodos = () => {
  open Js.Promise;
  Js.log("Fetching...");

  let uri = resolveUri("todos");

  Fetch.fetch(uri)
  |> then_(Fetch.Response.json)
  |> then_(json => {
      switch(Types.TodoItemCollection.t_decode(json)) {
      | Ok(collection) => resolve(Api.store.dispatch(Api.SetItems(Some(collection))))
      | Error({Decco.message, _}) => {
          // TODO: Show an error
          Js.log("Error fetching todos: " ++ message);
          resolve(Api.store.dispatch(Api.SetItems(None)));
        }
      };
    })
  |> Js.Promise.catch((_error: error) => {
      Js.log("Error fetching todos");
      resolve(Api.store.dispatch(Api.SetItems(None)));
    })
  |> ignore;
  None;
};
