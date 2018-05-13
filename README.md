# ReasonReactWaffle.re

ReasonReactWaffle provides React HOC (Higher Order Component) to provide components an access to a [Waffle.re](https://github.com/ZuraJanaiNazayDa/WaffleRe) stream, it updates the consumers whenever a value is poured into the stream. It helps with implementing [Meiosis](https://meiosis.js.org/) for state management.

ReasonReactWaffle is Experimental.

## Example:

```reason
let str = ReasonReact.string;

module Counter = {
  type state = {count:int};
  type update = Inc | Dec;
  let defaultState = {count: 0};
  let updates = Waffle.scan((state, update) => switch (update){
    | Inc => {count: state.count+1}
    | Dec => {count: state.count-1}
    },defaultState);
  module C = {
    type sInput = update;
    type sOutput = state;
    let name = "counter";
    let stream = updates;
  };
  include ReasonReactWaffle.CreateConsumer(C);
};

let root = {
  <div>
    <Counter.Consumer>
    ...(({count}, pour) => {
      <div>
        <h1>(str(string_of_int(count)))</h1>
        <button onClick={ _e => send(Counter.Inc)}>(str("+"))</button>
        <button onClick={ _e => send(Counter.Dec)}>(str("-"))</button>
      </div>
    })
    </Counter.Consumer>
  </div>
};

ReactDOMRe.renderToElementWithId(root, "target");
```

## Installation

```sh
npm install --save reason-react-waffle
```

Then add `reason-react-waffle` to `bs-dependencies` in your `bsconfig.json`:

```js
{
  ...
  "bs-dependencies": ["reason-react-waffle"]
}
```

### More examples are on the way :smile:
