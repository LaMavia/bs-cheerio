type t;
type tNull = Js.Nullable.t(t);

[@bs.module] external load: tNull => tNull = "cheerio";

[@bs.send] external text0: tNull => Js.Nullable.t(string) = "text";
[@bs.send] external text1: (tNull, string) => unit = "text";

[@bs.send] external html: tNull => Js.Nullable.t(string) = "html";
[@bs.send] external val_: tNull => Js.Nullable.t(string) = "val";
[@bs.send] external first: tNull => tNull = "first";
[@bs.get] external length: tNull => int = "length";

[@bs.send] external get0: tNull => tNull = "get";
[@bs.send] external get1: (tNull, int) => tNull = "get";

[@bs.send] external attr1: (tNull, string) => Js.Nullable.t(string) = "attr";
[@bs.send] external attr2: (tNull, string, 'a) => tNull = "attr";

[@bs.send] external prop1: (tNull, string) => 'a = "prop";
[@bs.send] external prop2: (tNull, string, 'a) => tNull = "prop";

[@bs.send] external parent: tNull => tNull = "parent";
[@bs.send] external next0: tNull => tNull = "next";
external next: (tNull, array(string)) => tNull = "next";

[@bs.send] external prev0: tNull => tNull = "prev";
external prev: (tNull, array(string)) => tNull = "prev";

[@bs.send] external contents: tNull => tNull = "contents";
[@bs.send] external each: (tNull, (int, tNull) => 'a) => unit = "each";
[@bs.send] external map: (tNull, (int, tNull) => 'a) => tNull = "map";
[@bs.send] external toArray: tNull => array('a) = "toArray";
// [@bs.send] external map

type node_type =
  | Element
  | Text
  | CData
  | PROCESSING_INSTRUCTION
  | Comment
  | Document
  | DocumentType
  | DocumentFragment
  | Deprecated;

[@bs.get] external nodeType: tNull => int = "nodeType";
let decodeNodeType = type_index =>
  switch (type_index) {
  | 1 => Element
  | 3 => Text
  | 4 => CData
  | 7 => PROCESSING_INSTRUCTION
  | 8 => Comment
  | 9 => Document
  | 10 => DocumentType
  | 11 => DocumentFragment
  | _ => Deprecated
  };

type element_type =
  | Comment
  | Tag
  | Text
  | Script
  | Style
  | Directive
  | Unknown_;

// [@bs.get] external type_: tNull => string = "type";
let type_: tNull => string = [%bs.raw
  {|
  x => {
    if(!x || x.length == 0) return ""
    if(x[0]) return x[0].type
    return x.type
  }
|}
];
let decodeType = t_ =>
  switch (t_) {
  | "comment" => Comment
  | "tag" => Tag
  | "text" => Text
  | "script" => Script
  | "style" => Style
  | "directive" => Directive
  | _ => Unknown_
  } /* comment, tag, text, script, style, directiv*/;