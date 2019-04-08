import tomd
from tomd import Tomd

string = """
<h1>h1</h1>
<h2>h2</h2>
<h3>h3</h3>
<h4>h4</h4>
<h5>h5</h5>
<h6>h6</h6>
<p>paragraph
<a href="https://github.com">link</a>
<img src="https://github.com" class="dsad">img</img>
<img src="https://github.com" class="dsad"/>
<img src="https://github.com" class="dsad">
</p>
<ul>
<li>1</li>
<li>2</li>
<li>3</li>
</ul>
<ol>
<li>1</li>
<li>2</li>
<li>3</li>
</ol>
<blockquote>blockquote</blockquote>
<p><code>inline code</code></p>
<pre><code>block code</code></pre>
<p>
<del>del</del>
<b>bold</b>
<i>italic</i>
<b><i>bold italic</i></b>
<em>em</em>
<strong>strong</strong>
aa <strong> strong   </strong> aa
</p>

<hr/>

<table>
<thead>
<tr class="1">
<th>th1</th>
<th>th2</th>
</tr>
</thead>
<tbody>
<tr>
<td>td</td>
<td>td</td>
</tr>
<tr>
<td>td</td>
<td>td</td>
</tr></tbody></table>

"""

print(Tomd(string).markdown)
print(tomd.convert(string))
