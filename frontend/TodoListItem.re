open Api;

module StyledTodoListItem = [%styled {|
  height: 3rem;
  width: 40rem;
  background-color: #eee;
  border: 1px solid #ccc;
  margin-bottom: 2rem;
|}];

module TodoTitle = [%styled (~decoration) => {j|
  height: 2rem;
  line-height: 2rem;
  font-size: 2rem;
  text-decoration: $decoration;
|j}];

[@react.component]
let make = (~id: int) => {
  let item = store.useStoreWithSelector(state => List.find(
    (item: Types.TodoItem.t) => item.id == id, state.items
  ), ());
  let onClick = (_event) => {
    Effects.toggleItem(id);
  };

  let onDeleteClick = (event) => {
    Effects.deleteItem(id);
    ReactEvent.Mouse.stopPropagation(event);
  };

  let decoration = item.complete ? "line-through" : "none";

  <li>
    <StyledTodoListItem onClick>
      <TodoTitle decoration>
        {React.string(item.title)}
      </TodoTitle>
      <span>
        {React.string(item.sub_text)}
      </span>
      <button onClick=onDeleteClick>(React.string("X"))</button>
    </StyledTodoListItem>
  </li>
};
