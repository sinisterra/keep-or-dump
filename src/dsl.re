type choice =
  | Keep
  | Dump;

type boardState =
  | Ready
  | Playing
  | Win
  | Loss;

type play = {
  question: int,
  choice,
};

type board = {
  options: list(int),
  boardState,
};

type item = {
  id: int,
  label: string,
  img: string,
};

type question = {
  id: int,
  q: string,
  keep: list(int),
};

type category = {
  items: list(item),
  title: string,
  questions: list(question),
};