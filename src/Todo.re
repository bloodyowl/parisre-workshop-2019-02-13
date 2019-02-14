open Belt;

type item = {
  id: string,
  text: string,
  checked: bool,
};

type filter =
  | All
  | Checked
  | Todo;

type state = {
  todos: array(item),
  input: string,
  filter,
};

type action =
  | SetInput(string)
  | CreateTodo
  | SetTodoStatus(string, bool)
  | DeleteTodo(string)
  | SetFilter(filter);

let component = ReasonReact.reducerComponent("Todo");

module Styles = {
  open Css;
  let input =
    style([
      borderWidth(zero),
      backgroundColor("eee"->hex),
      borderRadius(5->px),
      padding(10->px),
      width(100.->pct),
      boxSizing(borderBox),
      placeholder([opacity(0.3)]),
      hover([backgroundColor("ddd"->hex)]),
    ]);
  let todo =
    style([
      padding(10->px),
      fontFamily("sans-serif"),
      display(flexBox),
      flexDirection(row),
    ]);
  let checkedTodo = merge([todo, style([textDecoration(lineThrough)])]);
  let text = style([flexGrow(1.0)]);
};

let make = _ => {
  ...component,
  initialState: () => {todos: [||], input: "", filter: All},
  reducer: (action, state) =>
    switch (action) {
    | SetInput(input) => Update({...state, input})
    | CreateTodo =>
      if (state.input->Js.String.trim == "") {
        NoUpdate;
      } else {
        Update({
          ...state,
          todos:
            state.todos
            ->Array.concat([|
                {
                  text: state.input->Js.String.trim,
                  checked: false,
                  id: Js.String.make(Js.Math.random()),
                },
              |]),
          input: "",
        });
      }
    | SetTodoStatus(id, checked) =>
      Update({
        ...state,
        todos:
          state.todos
          ->Array.map(item => item.id == id ? {...item, checked} : item),
      })
    | DeleteTodo(id) =>
      Update({
        ...state,
        todos: state.todos->Array.keep(item => item.id != id),
      })
    | SetFilter(filter) => Update({...state, filter})
    },
  render: ({state, send}) =>
    <div>
      <input
        className=Styles.input
        placeholder="Type your todo"
        value={state.input}
        onChange={event =>
          send(SetInput(event->ReactEvent.Form.target##value))
        }
        onKeyUp={event =>
          if (event->ReactEvent.Keyboard.key == "Enter") {
            send(CreateTodo);
          }
        }
      />
      {state.todos
       ->Array.keepMap(item =>
           switch (state.filter, item) {
           | (All, _)
           | (Checked, {checked: true})
           | (Todo, {checked: false}) =>
             Some(
               <div
                 key={item.id}
                 className={item.checked ? Styles.checkedTodo : Styles.todo}>
                 <input
                   type_="checkbox"
                   checked={item.checked}
                   onChange={_ =>
                     send(SetTodoStatus(item.id, !item.checked))
                   }
                 />
                 <div className=Styles.text>
                   item.text->ReasonReact.string
                 </div>
                 <div
                   role="button"
                   tabIndex=0
                   onClick={_ => send(DeleteTodo(item.id))}>
                   {js|❌|js}->ReasonReact.string
                 </div>
               </div>,
             )
           | _ => None
           }
         )
       ->ReasonReact.array}
      <button onClick={_ => send(SetFilter(All))}>
        "All"->ReasonReact.string
      </button>
      <button onClick={_ => send(SetFilter(Checked))}>
        "Done"->ReasonReact.string
      </button>
      <button onClick={_ => send(SetFilter(Todo))}>
        "Todo"->ReasonReact.string
      </button>
    </div>,
};
