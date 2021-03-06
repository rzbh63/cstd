import os
from html.parser import HTMLParser


class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.jj=[]
        self.ind=0
        self.jj.append({'ind':self.ind, 'tag':'text', 'attrs':{'data':''}})
        self.tag_stack=['']
        self.ind=1

    def handle_starttag(self, tag, attrs):
        self.tag_stack.append((tag, self.ind))
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]
        self.jj.append({'ind':self.ind, 'tag':tag, 'attrs':attrs_dict})
        self.ind+=1
        

    def handle_endtag(self, tag):
        end=len(self.tag_stack)-3
        end=max(end, 0)
        for i in range(len(self.tag_stack)-1, end, -1):
            if self.tag_stack[i][0]==tag:
                self.ind=self.tag_stack[i][1]
                self.tag_stack.pop()
                break

        return

    def handle_data(self, data):
        self.jj.append({'ind':self.ind, 'tag':'text', 'attrs':{'data':data}})


def parser_x(parser, jj, i, l, out):
    tag='text'
    if jj[i]['tag'] in parser:
        tag = jj[i]['tag']

    return parser[tag](parser, jj, i, l, out)

def parser_text(parser, jj, i, l, out):
    text=''
    attrs = jj[i]['attrs']
    if 'data' in attrs:
        if 'in_code' in out and out['in_code']==1:
            text += attrs['data']
        else:
            if len(attrs['data'].strip())>0:
                text += attrs['data']

    ind=jj[i]['ind']
    ii = i+1
    text1=''
    while(ii<l):
        if jj[ii]['ind']<=ind:
            break
        t, ii = parser_x(parser, jj, ii, l, out)
        text1 += t

    if jj[i]['tag']=='a' and 'href' in attrs:
        text1 = '['+text1.strip()+']('+attrs['href']+')'

    text += text1
    return text, ii

def parser_code_text(parser, jj, i, l, out):
    out['in_code'] = 1
    text, ii = parser_text(parser, jj, i, l, out)
    out['in_code'] = 0
    return text, ii

def toline(text):
    return text.replace("\r", "").replace("\n", "")

def parser_skip(parser, jj, i, l, out):
    ind=jj[i]['ind']
    ii = i+1
    while(ii<l):
        if jj[ii]['ind']<=ind:
            break
        ii+=1
    return '', ii

def parser_inline(parser, jj, i, l, out, beg, end):
    text, ii =parser_text(parser, jj, i, l, out)
    text = beg + toline(text) + end
    return text, ii

def parser_h1(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n# ', '\n')

def parser_h2(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n## ', '\n')

def parser_h3(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n### ', '\n')

def parser_h4(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n#### ', '\n')

def parser_h5(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n##### ', '\n')

def parser_h6(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n###### ', '\n')

def parser_br(parser, jj, i, l, out):
    return '\n', i+1

def parser_mathjax_inline(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '$', '$')

def parser_mathjax_display(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '$', '$')

def parser_block(parser, jj, i, l, out, beg, end):
    text, ii = parser_text(parser, jj, i, l, out)
    text = beg + (text) + end
    return text, ii

def parser_p(parser, jj, i, l, out):
    return parser_block(parser, jj, i, l, out, '\n', '\n')

def parser_code_block(parser, jj, i, l, out, lang):
    text, ii = parser_code_text(parser, jj, i, l, out)
    text = '\n```'+lang+'\n' + (text.strip()) + '\n```\n'
    return text, ii


def parser_code(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        cc=attrs_dict['class']
        for lang in ['java', 'python', 'php', 'cpp', 'matlab', 'bash', 'xml']:
            if cc.find(lang)>=0:
                return parser_code_block(parser, jj, i, l, out, lang)

        for lang in ['language-py']:
            if cc.find(lang)>=0:
                return parser_code_block(parser, jj, i, l, out, 'python')

    return parser_code_block(parser, jj, i, l, out, '')


def parser_span(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        if (attrs_dict['class'] in ['mo', 'mi']):
            return parser_skip(parser, jj, i, l, out)

        if attrs_dict['class'] in ['katex--display', 'katex-display']:
            return parser_mathjax_display(parser, jj, i, l, out)
            
        if attrs_dict['class'] in ['katex--inline', 'katex-inline']:
            return parser_mathjax_inline(parser, jj, i, l, out)

    if 'id' in attrs_dict:
        if attrs_dict['id'].find("MathJax")>=0:
            return parser_skip(parser, jj, i, l, out)

    return parser_inline(parser, jj, i, l, out, '', '')


def parser_script(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'type' in attrs_dict:
        if attrs_dict['type']=='math/tex':
            return parser_mathjax_inline(parser, jj, i, l, out)

        if attrs_dict['type']=='math/tex; mode=display':
            return parser_mathjax_display(parser, jj, i, l, out)

    return parser_skip(parser, jj, i, l, out)

def parser_li(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '- ', '\n')

def parser_em(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '*', '*')

def parser_strong(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '**', '**')

def parser_del(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '~~', '~~')

def parser_blockquote(parser, jj, i, l, out):
    return parser_block(parser, jj, i, l, out, '\n> ', '\n')

def parser_div(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        if attrs_dict['class'] in ['article-copyright', 'mylinks', 'blogStats', 'header', 'postTitle', 'author', 'article-title-box']:
            return parser_skip(parser, jj, i, l, out)

    text, ii = parser_text(parser, jj, i, l, out)
    return text, ii
    #return parser_text(parser, jj, i, l, out)

def parser_title(parser, jj, i, l, out):
    text, ii = parser_text(parser, jj, i, l, out)
    text = toline(text).strip()
    out['title']=text
    text = '# ' + text + '\n'
    return text, ii

def node_find_tag(jj, i, l, tag):
    out=[]

    ind=jj[i]['ind']
    for ii in range(i+1, l):
        if jj[ii]['ind']<=ind:
            break
        if jj[ii]['tag']==tag:
            out.append(ii)

    return out

def parser_tr(parser, jj, ths, l, out, cols):
    text='|'
    for th in ths:
        text1, i = parser_inline(parser, jj, th, l, out, '', '')
        text += text1.strip() + '|'
    for j in range(len(ths), cols):
        text += ' |'
    return text, i

def parser_table(parser, jj, i, l, out):
    text = ''
    trs = node_find_tag(jj, i, l, 'tr')
    if len(trs)>0:
        ths = node_find_tag(jj, trs[0], l, 'th')
        if len(ths)==0:
            ths = node_find_tag(jj, trs[0], l, 'td')
        rows=[]
        cols = len(ths)

        for tr in trs[1::]:
            tds = node_find_tag(jj, tr, l, 'td')
            rows.append(tds)
            cols = max(len(tds), cols)

        if cols>0:
            text1, i = parser_tr(parser, jj, ths, l, out, cols)
            text += text1 + '\n'
            text += '|----'*cols
            text += '|\n'
            for tds in rows:
                text1,i = parser_tr(parser, jj, tds, l, out, cols)
                text += text1 + '\n'
    
    return text, i

def parser_img(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'src' in attrs_dict and len(attrs_dict['src'].strip())>0:
        alt = ''
        if 'alt' in attrs_dict:
            alt=attrs_dict['alt']
        href=attrs_dict['src']
        text = '!['+alt+']('+href+')'
        return text, i+1
    return parser_skip(parser, jj, i, l, out)


parser={
    'title': parser_title,
    'table': parser_table,
    'img': parser_img,
    'div': parser_div,
    'li': parser_li,
    'em': parser_em,
    'i': parser_em,
    'b': parser_strong,
    'strong': parser_strong,
    'del': parser_del,
    'p': parser_p,
    'blockquote': parser_blockquote,
    'br': parser_br,
    'h1': parser_h1,
    'h2': parser_h2,
    'h3': parser_h3,
    'h4': parser_h4,
    'h5': parser_h5,
    'h6': parser_h6,
    'text': parser_text,
    'code': parser_code,
    'span': parser_span,
    'style': parser_skip,
    'script': parser_script
}


import chardet
import win32clipboard as w
import win32con
import json

def autotext(t):
    codec = chardet.detect(t)
    return t.decode(codec['encoding'])


def node_filter(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1
    
    if tag=='div' and 'class' in attrs and attrs['class']=='blog-content-box':
        return 0
    
    if tag=='article' and 'class' in attrs and attrs['class']=='baidu_pl':
        return 0
        
    attrs_dict = attrs
    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='hide-article-box hide-article-pos text-center':
        return 1

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='postDesc':
        return 1

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='show-foot':
        return 1
    return 0

def node_find(jj, i, l):
    out=[]

    ind=jj[i]['ind']
    for ii in range(i+1, l):
        if jj[ii]['ind']<=ind:
            break
        if node_filter(jj[ii])==1:
            out.append(ii)

    return out


def savetext(fn, d):
    f=open(fn,'wt', encoding='utf8')
    f.write(d)
    f.close()
    #print(d)

def htm2md(t):
    html_code = t.replace('\r', '')
    hp = MyHTMLParser()
    hp.feed(html_code)
    jj=hp.jj
    hp.close()
    jjs = node_find(jj, 0, len(jj))
    d = ''
    out={'title':'title'}
    if len(jjs)>=2:
        i, l=jjs[0], jjs[1]
        while(i<l):
            text, i = parser_x(parser, jj, i, l, out)
            d += text

    return out['title'], d

if __name__ == '__main__':
    fn = './test1.html'
    f=open(fn,'rb')
    t=f.read()
    t = autotext(t)
    f.close()

    title, d = htm2md(t)
    print(title)
    fn2 = './test.md'
    f=open(fn2,'wt',encoding='utf8')
    f.write(d)
    f.close()
    #print(d)


