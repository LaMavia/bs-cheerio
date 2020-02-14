open Jest;
open Expect;

module Helpers = {
  let exn_of_nullable = n => n->Js.Nullable.toOption->Belt.Option.getExn;
  let def_of_nullable = (n, def) =>
    n->Js.Nullable.toOption->Belt.Option.getWithDefault(def);
};

let html = {|
  <!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <meta http-equiv="X-UA-Compatible" content="ie=edge"/>
  <title>Document</title>
</head>
<body>
  <input type="text" name="text_input" id="input-text" value="text"/>
  <ul className="ul_class">
    <li class="ul_class__li_class" id="LI_0_id">LI_0</li>
    <li class="ul_class__li_class" id="LI_1_id" data-d="LI_1_data">LI_1</li>
    <li class="ul_class__li_class">LI_2</li>
    <li class="ul_class__li_class"></li>
  </ul>
</body>
</html>
|};

describe("text", () => {
  let dom = Cheerio.load(html);
  test("with text", () => {
    dom
    ->Cheerio.select("ul li#LI_0_id")
    ->Element.text0
    ->Js.Nullable.toOption
    ->expect
    |> toBe(Some("LI_0"))
  });

  test("without text", () => {
    dom
    ->Cheerio.select("ul li:nth-child(4)")
    ->Element.text0
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some(""))
  });

  test("replaces text", () => {
    let el = dom->Cheerio.select("li#LI_0_id");
    let new_text = "new text";
    el->Element.text1(new_text);
    expect(el->Element.text0->Js.Nullable.toOption->Belt.Option.getExn)
    |> toEqual(new_text);
  });
});

describe("html", () => {
  let dom = Cheerio.load(html);
  test("works", () => {
    switch (dom->Cheerio.select("ul")->Element.html->Js.Nullable.toOption) {
    | Some(c) =>
      c
      |> expect
      |> toContainString({|<li class="ul_class__li_class">LI_2</li>|})
    | None => expect(false) |> toEqual(true)
    }
  });
});

describe("val", () => {
  let dom = Cheerio.load(html);
  test("works", () => {
    dom
    ->Cheerio.select("input[name=text_input]")
    ->Element.val_
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("text"))
  });
});

describe("first", () => {
  let dom = Cheerio.load(html);
  test("works", () => {
    dom
    ->Cheerio.select("ul li")
    ->Element.first
    ->Element.text0
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("LI_0"))
  });
});

describe("length", () => {
  let dom = Cheerio.load(html);
  test("returns the number of matched items", () => {
    dom->Cheerio.select("li")->Element.length |> expect |> toEqual(4)
  });
});

describe("get", () => {
  let dom = Cheerio.load(html);

  test("returns all the elements", () => {
    dom->Cheerio.select("li")->Element.get0->Element.length
    |> expect
    |> toEqual(4)
  });

  test("returns i-th element when the index is given", () => {
    dom
    ->Cheerio.select("li")
    ->Element.get1(1)
    ->Element.load
    ->Element.attr1("data-d")
    ->Helpers.def_of_nullable("")
    |> expect
    |> toEqual("LI_1_data")
  });
});

describe("attr1", () => {
  let dom = Cheerio.load(html);
  test("Some(p) when a prop exists", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr1("data-d")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("LI_1_data"))
  });
  test("None when a prop doesn't exist", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr1("made-up-prop")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
  test("None when an element doesn't exist", () => {
    dom
    ->Cheerio.select("li#IDontExist")
    ->Element.attr1("me-neither")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
});

describe("attr2", () => {
  let dom = Cheerio.load(html);
  test("Sets value of an existing element", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr2("data-sth", "some-value")
    ->Element.attr1("data-sth")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("some-value"))
  });
  test("None when trying to set a value of a nonexisting element", () => {
    dom
    ->Cheerio.select("li#IDontExist")
    ->Element.attr2("data-sth", "some-value")
    ->Element.attr1("data-sth")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
  test("Removes the attr if `null` is passed", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr2("data-d", [%bs.raw "null"])
    ->Element.attr1("data-d")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
});

describe("next", () => {
  let dom = Cheerio.load(html);
  test("Some when found the next node", () => {
    dom
    ->Cheerio.select("li#LI_0_id")
    ->Element.next0
    ->Element.attr1("id")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("LI_1_id"))
  });
});

describe("properties", () => {
  let dom = Cheerio.load(html);

  test("type", () => {
    dom->Cheerio.select("ul")->Element.type_->Element.decodeType
    |> expect
    |> toEqual(Element.Tag)
  });
});

describe("each", () => {
  let dom = Cheerio.load(html);

  test("iterates over each element", () => {
    let visited = ref([false, false, false]);
    dom
    ->Cheerio.select("ul")
    ->Element.contents
    ->Element.each((i, _) => {
        // el->Element.load->Element.type_->Js.Console.log;
        visited := (visited^)->Belt.List.mapWithIndex((j, v) => v || j == i)
      });
    expect(visited^) |> toEqual([true, true, true]);
  });
});

describe("map", () => {
  let dom = Cheerio.load(html);

  test("maps over items", () => {
    dom
    ->Cheerio.select("li")
    ->Element.map((_, el) => el->Element.load->Element.text0)
    ->Element.toArray
    |> expect
    |> toEqual([|"LI_0", "LI_1", "LI_2", ""|])
  });
});

describe("toArray", () => {
  let dom = Cheerio.load(html);

  test("returns a list", () => {
    dom->Cheerio.select("li")->Element.toArray->Belt.Array.length
    |> expect
    |> toEqual(4)
  });
});