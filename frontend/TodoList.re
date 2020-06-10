open Api;

[@react.component]
let make = () => {
  let state = store.useStore();

  React.useEffect1(Effects.fetchTodos, [||]);

  <>
    <ul>
      {
        state.items
          |> List.map((item: Types.TodoItem.t) => <TodoListItem key=item.title id=item.id />)
          |> Array.of_list
          |> React.array
      }
    </ul>
    <AddTodoButton />
  </>
};
