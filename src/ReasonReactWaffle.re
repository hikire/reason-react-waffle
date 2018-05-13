module type Config = {
  type sInput;
  type sOutput;
  let name: string;
  let stream: Waffle.t(sInput, sOutput);
};

module CreateConsumer = (C: Config) => {
  let listners: ref(list('b => unit)) = ref([]);
  let listen = (listner: 'b => unit) => {
    listners := [listner, ...listners^];
    () => listners := (listners^ |> Belt.List.keep) @@ (l => l == listner);
  };
  let notifyListners = (value: 'b) =>
    Belt.List.forEach(listners^, fn => fn(value));
  (notifyListners |> Waffle.on) @@ C.stream;
  module Consumer = {
    type state = {value: option(C.sOutput)};
    type action =
      | SetValue(C.sOutput);
    let component = ReasonReact.reducerComponent(C.name ++ "Consumer");
    let make = children => {
      ...component,
      initialState: () => {value: C.stream#peek()},
      reducer: (action, _state) =>
        switch (action) {
        | SetValue(v) => ReasonReact.Update({value: Some(v)})
        },
      render: ({state: {value}}) =>
        switch (value) {
        | Some(v) => children(v, C.stream#pour)
        | None => ReasonReact.null
        },
      subscriptions: self => [
        Sub(() => listen(v => self.send(SetValue(v))), unSub => unSub()),
      ],
    };
  };
};