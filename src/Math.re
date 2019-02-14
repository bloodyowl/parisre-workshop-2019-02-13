[@bs.module "./math.js"] external double: int => int = "double";
[@bs.module "./math.js"]
external double_float: float => float = "double";

Js.log(double(2));
Js.log(double_float(2.3));

// type changeEvent;
// [@bs.module "./event-emitter"]
// external onChange: ([@bs.as "change"] _, changeEvent => unit) => unit = "on";

// onChange(x => Js.log(x));

let f = [1, 2, 3, 4, 5];

[@bs.deriving jsConverter]
type pricing = [ | `view | `click];

type pricingView = [ | `view];

let treatClick =
  fun
  | `click => true;

type user = {
  id: string,
  name: string,
};

let georges = {id: "1", name: "Georges"};

let getOtherGeorges = georges => {...georges, id: "2"};
