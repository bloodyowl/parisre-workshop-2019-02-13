type state;

type action;

let make:
  'a => ReasonReact.component(state, ReasonReact.noRetainedProps, action);
