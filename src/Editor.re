open Dsl;
type action =
  | UpdateCategoryTitle(string)
  | AddItemToCurrentCategory
  | UpdateCategoryItem(int, item)
  | AddQuestion
  | UpdateQuestion(question)
  | LoadBoard(board);

let str = ReasonReact.string;

let getValue = event => ReactEvent.Form.target(event)##value;

type state = {
  questionCount: int,
  itemCount: int,
  plays: list(play),
  currentCategory: category,
  categories: list(category),
  playCounter: int,
  board,
};

let makeBlankCategoryItem = (id: int): item => {
  id,
  label: "Item " ++ string_of_int(id),
  img: "",
};

let makeBlankQuestion = (id: int): question => {id, q: "", keep: []};

let applyPlay = (category: category, board: board, play: play): board => {
  Js.log((category, board, play));
  board;
};

let reducer = (action, state) => {
  switch (action) {
  | UpdateCategoryTitle(title) =>
    ReasonReact.Update({
      ...state,
      currentCategory: {
        ...state.currentCategory,
        title,
      },
    })
  | AddItemToCurrentCategory =>
    ReasonReact.Update({
      ...state,
      itemCount: state.itemCount + 1,
      currentCategory: {
        ...state.currentCategory,
        items:
          state.currentCategory.items
          @ [makeBlankCategoryItem(state.itemCount)],
      },
    })

  | UpdateCategoryItem(id, newItem) =>
    ReasonReact.Update({
      ...state,
      currentCategory: {
        ...state.currentCategory,
        items:
          state.currentCategory.items
          |> List.map((currentItem: item) =>
               id == currentItem.id ? newItem : currentItem
             ),
      },
    })
  | AddQuestion =>
    ReasonReact.Update({
      ...state,
      questionCount: state.questionCount + 1,
      currentCategory: {
        ...state.currentCategory,
        questions:
          state.currentCategory.questions
          @ [makeBlankQuestion(state.questionCount)],
      },
    })
  | UpdateQuestion(question) =>
    ReasonReact.Update({
      ...state,
      currentCategory: {
        ...state.currentCategory,
        questions:
          state.currentCategory.questions
          |> List.map(q =>
               q.id == question.id ? {...question, id: question.id} : q
             ),
      },
    })
  | LoadBoard(board) => ReasonReact.Update({...state, board})
  };
};

let initialState: state = {
  itemCount: 10,
  questionCount: 5,
  categories: [],
  plays: [{question: 1, choice: Keep}, {question: 2, choice: Dump}],
  playCounter: 0,
  board: {
    options: [],
    boardState: Ready,
  },
  currentCategory: {
    questions: [
      {id: 1, q: "Is Prime?", keep: [2, 3, 5, 7]},
      {id: 2, q: "Is divisible by 2", keep: [2, 4, 6, 8, 10]},
      {id: 3, q: "Less or equal than 3", keep: [1, 2, 3]},
      {id: 4, q: "Divisible by 3", keep: [3, 6, 9]},
      {id: 5, q: "Integer bounds of Pi", keep: [3, 4]},
    ],
    items:
      [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
      |> List.map(id => {id, label: string_of_int(id), img: ""}),
    title: "Numbers",
  },
};

let component = ReasonReact.reducerComponent("Editor");

let make = _children => {
  ...component,
  reducer,
  initialState: () => initialState,
  didMount: self => {
    let board = {
      options:
        List.map(item => item.id, self.state.currentCategory.questions),
      boardState: Ready,
    };
    self.send(LoadBoard(board));
  },
  render: self => {
    let {currentCategory, board, playCounter, plays} = self.state;

    let {title, items, questions} = currentCategory;

    let displayTitle = title == "" ? "(No title)" : title;

    let addItem = _ => self.send(AddItemToCurrentCategory);

    let totalItems = List.length(items);
    let gridWidth = totalItems |> float_of_int |> sqrt |> ceil |> int_of_float;

    let updateQuestion = (question: question) => {
      self.send(UpdateQuestion(question));
    };

    let addQuestion = _ => self.send(AddQuestion);

    let updateCategoryItem = (index, item) =>
      self.send(UpdateCategoryItem(index, item));

    let playCounterBack = _e => {
      Js.log(playCounter - 1);
      ();
    };
    let playCounterForward = _e => {
      Js.log(playCounter + 1);
      ();
    };

    let loadCategoryToBoard = _e => {
      Js.log("loading...");
      let emptyBoard: board = {
        options: List.map((item: item) => item.id, items),
        boardState: Ready,
      };
      Js.log(emptyBoard);
    };

    <>
      <h1> {"Current Category: " ++ displayTitle |> str} </h1>
      <button onClick=addItem> {"AddItem" |> str} </button>
      <button onClick=addQuestion> {"Add a Question" |> str} </button>
      <input
        type_="text"
        placeholder="Category title"
        onChange={e =>
          getValue(e) |> (s => self.send(UpdateCategoryTitle(s)))
        }
      />
      <div>
        {items
         |> List.mapi((i, item) =>
              <CategoryItem
                item
                index=i
                key={string_of_int(i)}
                editable=true
                updateCategoryItem={updateCategoryItem(item.id)}
                updateQuestion
                questions
              />
            )
         |> Array.of_list
         |> ReasonReact.array}
      </div>
      <div
        style={ReactDOMRe.Style.make(
          ~width="500px",
          ~display="grid",
          ~gridTemplateColumns=
            "repeat(" ++ string_of_int(gridWidth) ++ ", minmax(200px,1fr))",
          (),
        )}>
        {items
         |> List.mapi((i, item) =>
              <div
                key={string_of_int(i)}
                style={ReactDOMRe.Style.make(~border="1px solid cyan", ())}>
                {item.label |> str}
              </div>
            )
         |> Array.of_list
         |> ReasonReact.array}
      </div>
      <div
        style={ReactDOMRe.Style.make(
          ~display="flex",
          ~flexDirection="row",
          ~justifyContent="space-between",
          (),
        )}>
        <button onClick=playCounterBack disabled={playCounter == 0}>
          {" <- Back" |> str}
        </button>
        <div>
          {"Current play: " ++ string_of_int(playCounter) |> str}
          <button onClick=loadCategoryToBoard>
            {str("Load category")}
          </button>
        </div>
        <button onClick=playCounterForward> {"Forward ->" |> str} </button>
      </div>
      <Board board />
    </>;
  },
};