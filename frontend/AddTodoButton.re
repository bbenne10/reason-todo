[@react.component]
let make = () => {
    let (curTitle, setCurTitle) = React.useState(() => "");
    let (curSubText, setCurSubText) = React.useState(() => "");

  let onTitleChange = (event) => {
    let value = ReactEvent.Form.target(event)##value;
    setCurTitle(value);
  };

  let onSubTextChange = (event) => {
    let value = ReactEvent.Form.target(event)##value;
    setCurSubText(value);
  };

  let onClick = (_event) => {
    Effects.createItem(curTitle, curSubText);
    setCurTitle((_) => "");
    setCurSubText((_) => "");
  };

  <>
    <label htmlFor="title">(React.string("Title:"))</label>
    <input name="title" value=curTitle onChange=onTitleChange />

    <label htmlFor="subText">(React.string("Sub-text:"))</label>
    <input name="subText" value=curSubText onChange=onSubTextChange />

    <button onClick>(React.string("Add Todo"))</button>
  </>
}
