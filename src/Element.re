type t;
type tNull = Js.Nullable.t(t);

[@bs.send] external text: tNull => string = "text";
[@bs.send] external html: tNull => string = "html";

[@bs.send] external attr1: (tNull, string) => Js.Nullable.t(string) = "attr";
[@bs.send] external attr2: (tNull, string, 'a) => tNull = "attr";

[@bs.send] external prop1: (tNull, string) => 'a = "prop";
[@bs.send] external prop2: (tNull, string, 'a) => tNull = "prop";

[@bs.send] external parent: tNull => tNull = "parent";
[@bs.send] external next0: tNull => tNull = "next";
external next: (tNull, array(string)) => tNull = "next";

[@bs.send] external prev0: tNull => tNull = "prev";
external prev: (tNull, array(string)) => tNull = "prev";