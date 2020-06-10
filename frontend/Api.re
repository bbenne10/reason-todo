open Types;

type state = {
  items: list(TodoItem.t)
};

let emptyState: state = {
  items: []
};

type actions =
  | AddItem(TodoItem.t)
  | DeleteItem(int)
  | ToggleItem(int)
  | SetItems(option(TodoItemCollection.t));

let _add = (newItem: TodoItem.t, state: state) => {
  {
    ...state,
    items: [
      newItem,
      ...state.items
    ]
  }
};


let _toggle = (id: int, state: state) => {
  {...state,
   items: List.map(
    (item: Types.TodoItem.t) => {
       item.id == id ? {...item, complete: !item.complete} : item
    },
    state.items)
   }
};

let _set = (items: option(TodoItemCollection.t), _state: state) => {
  switch(items) {
    | Some(items) => {items: items.items}
    | None => emptyState
  }
};

let _delete = (id: int, state: state) => {
  {...state,
   items: state.items |> List.filter((item: TodoItem.t) => item.id != id)
  }
};


let reducer = (state, action) => {
  switch (action) {
  | AddItem(newItem) => _add(newItem, state)
  | DeleteItem(id) => _delete(id, state)
  | ToggleItem(id) => _toggle(id, state)
  | SetItems(items) => _set(items, state)
  };
};

let store = Restorative.createStore(emptyState, reducer);
