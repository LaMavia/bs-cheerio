type t;

[@bs.module "cheerio"] [@bs.val] external load: string => t = "load";
let select: (t, string) => Element.tNull = [%raw
  "($, selector) => $(selector)"
];
[@bs.module]
external selectWithContext: (string, string) => Js.Nullable.t(Element.tNull) = "cheerio";