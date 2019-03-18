type route =
  | Home
  | Comments(int)
  | Editor
  | NotFound;

type state = {route};

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["comments", id] => Comments(int_of_string(id))
  | ["editor"] => Editor
  | _ => NotFound
  };

let component = ReasonReact.reducerComponent("Routes");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: Home},
  didMount: self => {
    let watchId =
      ReasonReact.Router.watchUrl(url =>
        self.send(ChangeRoute(url |> mapUrlToRoute))
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
  },
  render: self =>
    switch (self.state.route) {
    | Home => <div> {ReasonReact.string("Hola")} </div>
    | Editor => <Editor />
    | Comments(id) =>
      <div> {ReasonReact.string("Comments" ++ string_of_int(id))} </div>
    | NotFound => <div> {ReasonReact.string("NPI")} </div>
    },
};