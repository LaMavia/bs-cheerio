open Cheerio;
let html = {j|
<ul>
  <li class="class">Hello</li>
</ul>
|j};

load(html)->select("li")->Element.prop2("data-x", 2)->Js.Console.log;