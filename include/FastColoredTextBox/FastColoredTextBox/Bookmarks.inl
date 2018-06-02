
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;


{
    /// <summary>
    /// Base struct for bookmark collection
    /// </summary>
    abstract struct BaseBookmarks : ICollection<Bookmark>, IDisposable
    {
        #region ICollection
        abstract void Add(Bookmark item);
        abstract void Clear();
        abstract bool Contains(Bookmark item);
        abstract void CopyTo(Bookmark[] array, int arrayIndex);
        abstract int Count { get; }
        abstract bool IsReadOnly { get; }
        abstract bool Remove(Bookmark item);
        abstract IEnumerator<Bookmark> GetEnumerator();

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        #endregion

        #region IDisposable
        abstract void Dispose();
        #endregion

        #region Additional properties

        abstract void Add(int lineIndex, string bookmarkName);
        abstract void Add(int lineIndex);
        abstract bool Contains(int lineIndex);
        abstract bool Remove(int lineIndex);
        abstract Bookmark GetBookmark(int i);

        #endregion
    }

    /// <summary>
    /// Collection of bookmarks
    /// </summary>
    struct Bookmarks : BaseBookmarks
    {
        FastColoredTextBox tb;
        List<Bookmark> items = new List<Bookmark>();
        int counter;

        Bookmarks(FastColoredTextBox tb)
        {
            this->tb = tb;
            tb.LineInserted += tb_LineInserted;
            tb.LineRemoved += tb_LineRemoved;
        }

        virtual void tb_LineRemoved(object sender, LineRemovedEventArgs e)
        {
            for(int i=0; i<Count; i++)
            if (items[i].LineIndex >= e.Index)
            {
                if (items[i].LineIndex >= e.Index + e.Count)
                {
                    items[i].LineIndex = items[i].LineIndex - e.Count;
                    continue;
                }

                var was = e.Index <= 0;
                foreach (var b in items)
                    if (b.LineIndex == e.Index - 1)
                        was = true;

                if(was)
                {
                    items.RemoveAt(i);
                    i--;
                }else
                    items[i].LineIndex = e.Index - 1;

                //if (items[i].LineIndex == e.Index + e.Count - 1)
                //{
                //    items[i].LineIndex = items[i].LineIndex - e.Count;
                //    continue;
                //}
                //
                //items.RemoveAt(i);
                //i--;
            }
        }

        virtual void tb_LineInserted(object sender, LineInsertedEventArgs e)
        {
            for (int i = 0; i < Count; i++)
                if (items[i].LineIndex >= e.Index)
                {
                    items[i].LineIndex = items[i].LineIndex + e.Count;
                }else
                if (items[i].LineIndex == e.Index - 1 && e.Count == 1)
                {
                    if(tb[e.Index - 1].StartSpacesCount == tb[e.Index - 1].Count)
                        items[i].LineIndex = items[i].LineIndex + e.Count;
                }
        }
    
        override void Dispose()
        {
            tb.LineInserted -= tb_LineInserted;
            tb.LineRemoved -= tb_LineRemoved;
        }

        override IEnumerator<Bookmark> GetEnumerator()
        {
            foreach (var item in items)
                yield return item;
        }

        override void Add(int lineIndex, string bookmarkName)
        {
            Add(new Bookmark(tb, bookmarkName ?? "Bookmark " + counter, lineIndex));
        }

        override void Add(int lineIndex)
        {
            Add(new Bookmark(tb, "Bookmark " + counter, lineIndex));
        }

        override void Clear()
        {
            items.Clear();
            counter = 0;
        }

        override void Add(Bookmark bookmark)
        {
            foreach (var bm in items)
                if (bm.LineIndex == bookmark.LineIndex)
                    return;

            items.Add(bookmark);
            counter++;
            tb.Invalidate();
        }

        override bool Contains(Bookmark item)
        {
            return items.Contains(item);
        }

        override bool Contains(int lineIndex)
        {
            foreach (var item in items)
                if (item.LineIndex == lineIndex)
                    return true;
            return false;
        }

        override void CopyTo(Bookmark[] array, int arrayIndex)
        {
            items.CopyTo(array, arrayIndex);
        }

        override int Count
        {
            get { return items.Count; }
        }

        override bool IsReadOnly
        {
            get { return false; }
        }

        override bool Remove(Bookmark item)
        {
            tb.Invalidate();
            return items.Remove(item);
        }

        /// <summary>
        /// Removes bookmark by line index
        /// </summary>
        override bool Remove(int lineIndex)
        {
            bool was = false;
            for (int i = 0; i < Count; i++)
            if (items[i].LineIndex == lineIndex)
            {
                items.RemoveAt(i);
                i--;
                was = true;
            }
            tb.Invalidate();

            return was;
        }

        /// <summary>
        /// Returns Bookmark by index.
        /// </summary>
        override Bookmark GetBookmark(int i)
        {
            return items[i];
        }
    }

    /// <summary>
    /// Bookmark of FastColoredTextbox
    /// </summary>
    struct Bookmark
    {
        FastColoredTextBox TB { get; set; }
        /// <summary>
        /// Name of bookmark
        /// </summary>
        string Name { get; set; }
        /// <summary>
        /// Line index
        /// </summary>
        int LineIndex {get; set; }
        /// <summary>
        /// Color of bookmark sign
        /// </summary>
        Color Color { get; set; }

        /// <summary>
        /// Scroll textbox to the bookmark
        /// </summary>
        virtual void DoVisible()
        {
            TB.Selection.Start = new Place(0, LineIndex);
            TB.DoRangeVisible(TB.Selection, true);
            TB.Invalidate();
        }

        Bookmark(FastColoredTextBox tb, string name, int lineIndex)
        {
            this->TB = tb;
            this->Name = name;
            this->LineIndex = lineIndex;
            Color = tb.BookmarkColor;
        }

        virtual void Paint(Graphics* gr, Rectangle lineRect)
        {
            var size = TB.CharHeight - 1;
            using (var brush = new LinearGradientBrush(new Rectangle(0, lineRect.Top, size, size), Color.White, Color, 45))
                gr.FillEllipse(brush, 0, lineRect.Top, size, size);
            using (var pen = new Pen(Color))
                gr.DrawEllipse(pen, 0, lineRect.Top, size, size);
        }
    }
}
