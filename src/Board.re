open Dsl;
let component = ReasonReact.statelessComponent("Board");

let make = (_children, ~board: board) => {
  ...component,
  render: _self => {
    Js.log(board);
    <div> {"Soy el tablero" |> ReasonReact.string} </div>;
  },
};