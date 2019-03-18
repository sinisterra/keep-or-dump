open Dsl;
let component = ReasonReact.statelessComponent("CategoryItem");

let make =
    (
      _children,
      ~item: item,
      ~editable: bool,
      ~index: int,
      ~updateCategoryItem: item => unit,
      ~updateQuestion: question => unit,
      ~questions: list(question),
    ) => {
  ...component,
  render: _self => {
    let {label, img} = item;

    let handleQuestionItemChange = (question, _) => {
      let itemIsInQuestion =
        question.keep |> List.exists(qId => qId == item.id);

      let keep =
        itemIsInQuestion ?
          List.filter(qId => qId != item.id, question.keep) :
          question.keep @ [item.id];

      Js.log((itemIsInQuestion, question.keep, keep));

      updateQuestion({...question, keep});
    };
    if (!editable) {
      <div> <img src=img /> {ReasonReact.string(label)} </div>;
    } else {
      <div
        style={ReactDOMRe.Style.make(
          ~display="flex",
          ~flexDirection="row",
          (),
        )}>
        <div>
          <label>
            {index
             + 1
             |> string_of_int
             |> (s => s ++ ". ")
             |> ReasonReact.string}
          </label>
          <input
            type_="text"
            value=label
            onChange={e =>
              updateCategoryItem({
                ...item,
                label: e->ReactEvent.Form.target##value,
              })
            }
          />
        </div>
        {questions
         |> List.map(question => {
              let itemInQuestionKeep =
                List.exists(id => id == item.id, question.keep);
              <div
                key={
                  string_of_int(item.id)
                  ++ "__"
                  ++ string_of_int(question.id)
                }
                style={ReactDOMRe.Style.make(
                  ~display="flex",
                  ~alignItems="center",
                  ~justifyContent="center",
                  (),
                )}>
                <input
                  type_="checkbox"
                  checked=itemInQuestionKeep
                  onChange={handleQuestionItemChange(question)}
                />
              </div>;
            })
         |> Array.of_list
         |> ReasonReact.array}
      </div>;
    };
  },
};