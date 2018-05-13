open Jest;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

type state = int;

type action =
  | Set(int);

describe("Consumer", () => {
  open Expect;
  let reducer = (_state, action: action) =>
    switch (action) {
    | Set(i) => i
    };
  let initialState = 0;
  let stateStream = Waffle.scan(reducer, initialState);
  module C = {
    type sInput = action;
    type sOutput = state;
    let name = "number";
    let stream = stateStream;
  };
  module Num = ReasonReactWaffle.CreateConsumer(C);
  let receivedValue = ref(-1);
  let consumer =
    <Num.Consumer>
      ...(
           (value, send) => {
             receivedValue := value;
             if (value == 2) {
               send(Set(3));
             };
             ReasonReact.null;
           }
         )
    </Num.Consumer>
    |> Enzyme.shallow;
  test("renders", () =>
    consumer |> Enzyme.exists |> expect |> toBe(true)
  );
  test("receives the value", () =>
    receivedValue^ |> expect |> toBe(0)
  );
  test("updates the value", () => {
    stateStream#pour(Set(1));
    receivedValue^ |> expect |> toBe(1);
  });
  test("can send actions", () => {
    stateStream#pour(Set(2));
    stateStream#peek() |> expect |> toEqual(Some(3));
  });
});