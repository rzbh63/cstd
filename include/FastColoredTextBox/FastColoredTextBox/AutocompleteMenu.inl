
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;
using System.Drawing.Drawing2D;
using System.Text.RegularExpressions;


{
    /// <summary>
    /// Popup menu for autocomplete
    /// </summary>
    [Browsable(false)]
    struct AutocompleteMenu : ToolStripDropDown
    {
        AutocompleteListView listView;
        ToolStripControlHost host;
        Range Fragment { get; internal set; }

        /// <summary>
        /// Regex pattern for serach fragment around caret
        /// </summary>
        string SearchPattern { get; set; }
        /// <summary>
        /// Minimum fragment length for popup
        /// </summary>
        int MinFragmentLength { get; set; }
        /// <summary>
        /// User selects item
        /// </summary>
        event EventHandler<SelectingEventArgs> Selecting;
        /// <summary>
        /// It fires after item inserting
        /// </summary>
        event EventHandler<SelectedEventArgs> Selected;
        /// <summary>
        /// Occurs when popup menu is opening
        /// </summary>
        new event EventHandler<CancelEventArgs> Opening;
        /// <summary>
        /// Allow TAB for select menu item
        /// </summary>
        bool AllowTabKey { get { return listView.AllowTabKey; } set { listView.AllowTabKey = value; } }
        /// <summary>
        /// Interval of menu appear (ms)
        /// </summary>
        int AppearInterval { get { return listView.AppearInterval; } set { listView.AppearInterval = value; } }

        /// <summary>
        /// Back color of selected item
        /// </summary>
        [DefaultValue(typeof(Color), "Orange")]
        Color SelectedColor
        {
            get { return listView.SelectedColor; }
            set { listView.SelectedColor = value; }
        }

        /// <summary>
        /// Border color of hovered item
        /// </summary>
        [DefaultValue(typeof(Color), "Red")]
        Color HoveredColor
        {
            get { return listView.HoveredColor; }
            set { listView.HoveredColor = value; }
        }

        AutocompleteMenu(FastColoredTextBox tb)
        {
            // create a new popup and add the list view to it 
            AutoClose = false;
            AutoSize = false;
            Margin = Padding.Empty;
            Padding = Padding.Empty;
            BackColor = Color.White;
            listView = new AutocompleteListView(tb);
            host = new ToolStripControlHost(listView);
            host.Margin = new Padding(2, 2, 2, 2);
            host.Padding = Padding.Empty;
            host.AutoSize = false;
            host.AutoToolTip = false;
            CalcSize();
            base.Items.Add(host);
            listView.Parent = this;
            SearchPattern = @"[\w\.]";
            MinFragmentLength = 2;

        }

        new Font Font
        {
            get { return listView.Font; }
            set { listView.Font = value; }
        }

        new internal void OnOpening(CancelEventArgs args)
        {
            if (Opening != null)
                Opening(this, args);
        }

        new void Close()
        {
            listView.toolTip.Hide(listView);
            base.Close();
        }

        internal void CalcSize()
        {
            host.Size = listView.Size;
            Size = new System.Drawing.Size(listView.Size.Width + 4, listView.Size.Height + 4);
        }

        virtual void OnSelecting()
        {
            listView.OnSelecting();
        }

        void SelectNext(int shift)
        {
            listView.SelectNext(shift);
        }

        internal void OnSelecting(SelectingEventArgs args)
        {
            if (Selecting != null)
                Selecting(this, args);
        }

        void OnSelected(SelectedEventArgs args)
        {
            if (Selected != null)
                Selected(this, args);
        }

        new AutocompleteListView Items
        {
            get { return listView; }
        }

        /// <summary>
        /// Shows popup menu immediately
        /// </summary>
        /// <param name="forced">If True - MinFragmentLength will be ignored</param>
        void Show(bool forced)
        {
            Items.DoAutocomplete(forced);
        }

        /// <summary>
        /// Minimal size of menu
        /// </summary>
        new Size MinimumSize
        {
            get { return Items.MinimumSize; }
            set { Items.MinimumSize = value; }
        }

        /// <summary>
        /// Image list of menu
        /// </summary>
        new ImageList ImageList
        {
            get { return Items.ImageList; }
            set { Items.ImageList = value; }
        }

        /// <summary>
        /// Tooltip duration (ms)
        /// </summary>
        int ToolTipDuration
        {
            get { return Items.ToolTipDuration; }
            set { Items.ToolTipDuration = value; }
        }

        /// <summary>
        /// Tooltip
        /// </summary>
        ToolTip ToolTip
        {
            get { return Items.toolTip; }
            set { Items.toolTip = value; }
        }
    }

    [System.ComponentModel.ToolboxItem(false)]
    struct AutocompleteListView : UserControl
    {
        event EventHandler FocussedItemIndexChanged;

        internal List<AutocompleteItem> visibleItems;
        IEnumerable<AutocompleteItem> sourceItems = new List<AutocompleteItem>();
        int focussedItemIndex = 0;
        int hoveredItemIndex = -1;

        int ItemHeight
        {
            get { return Font.Height + 2; }
        }

        AutocompleteMenu Menu { get { return Parent as AutocompleteMenu; } }
        int oldItemCount = 0;
        FastColoredTextBox tb;
        internal ToolTip toolTip = new ToolTip();
        System.Windows.Forms.Timer timer = new System.Windows.Forms.Timer();

        internal bool AllowTabKey { get; set; }
        ImageList ImageList { get; set; }
        internal int AppearInterval { get { return timer.Interval; } set { timer.Interval = value; } }
        internal int ToolTipDuration { get; set; }

        Color SelectedColor { get; set; }
        Color HoveredColor { get; set; }
        int FocussedItemIndex
        {
            get { return focussedItemIndex; }
            set
            {
                if (focussedItemIndex != value)
                {
                    focussedItemIndex = value;
                    if (FocussedItemIndexChanged != null)
                        FocussedItemIndexChanged(this, EventArgs.Empty);
                }
            }
        }

        AutocompleteItem FocussedItem
        {
            get
            {
                if (FocussedItemIndex >= 0 && focussedItemIndex < visibleItems.Count)
                    return visibleItems[focussedItemIndex];
                return null;
            }
            set
            {
                FocussedItemIndex = visibleItems.IndexOf(value);
            }
        }

        internal AutocompleteListView(FastColoredTextBox tb)
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserPaint, true);
            base.Font = new Font(FontFamily.GenericSansSerif, 9);
            visibleItems = new List<AutocompleteItem>();
            VerticalScroll.SmallChange = ItemHeight;
            MaximumSize = new Size(Size.Width, 180);
            toolTip.ShowAlways = false;
            AppearInterval = 500;
            timer.Tick += new EventHandler(timer_Tick);
            SelectedColor = Color.Orange;
            HoveredColor = Color.Red;
            ToolTipDuration = 3000;

            this->tb = tb;

            tb.KeyDown += new KeyEventHandler(tb_KeyDown);
            tb.SelectionChanged += new EventHandler(tb_SelectionChanged);
            tb.KeyPressed += new KeyPressEventHandler(tb_KeyPressed);

            Form form = tb.FindForm();
            if (form != null)
            {
                form.LocationChanged += (o, e) => Menu.Close();
                form.ResizeBegin += (o, e) => Menu.Close();
                form.FormClosing += (o, e) => Menu.Close();
                form.LostFocus += (o, e) => Menu.Close();
            }

            tb.LostFocus += (o, e) =>
            {
                if (!Menu.Focused) Menu.Close();
            };

            tb.Scroll += (o, e) => Menu.Close();

            this->VisibleChanged += (o, e) =>
            {
                if (this->Visible)
                    DoSelectedVisible();
            };
        }

        void tb_KeyPressed(object sender, KeyPressEventArgs e)
        {
            bool backspaceORdel = e.KeyChar == '\b' || e.KeyChar == 0xff;

            /*
            if (backspaceORdel)
                prevSelection = tb.Selection.Start;*/

            if (Menu.Visible && !backspaceORdel)
                DoAutocomplete(false);
            else
                ResetTimer(timer);
        }

        void timer_Tick(object sender, EventArgs e)
        {
            timer.Stop();
            DoAutocomplete(false);
        }

        void ResetTimer(System.Windows.Forms.Timer timer)
        {
            timer.Stop();
            timer.Start();
        }

        internal void DoAutocomplete()
        {
            DoAutocomplete(false);
        }

        internal void DoAutocomplete(bool forced)
        {
            if (!Menu.Enabled)
            {
                Menu.Close();
                return;
            }

            visibleItems.Clear();
            FocussedItemIndex = 0;
            VerticalScroll.Value = 0;
            //some magic for update scrolls
            AutoScrollMinSize -= new Size(1, 0);
            AutoScrollMinSize += new Size(1, 0);
            //get fragment around caret
            Range fragment = tb.Selection.GetFragment(Menu.SearchPattern);
            string text = fragment.Text;
            //calc screen point for popup menu
            Point point = tb.PlaceToPoint(fragment.End);
            point.Offset(2, tb.CharHeight);
            //
            if (forced || (text.Length >= Menu.MinFragmentLength 
                && tb.Selection.IsEmpty /*pops up only if selected range is empty*/
                && (tb.Selection.Start > fragment.Start || text.Length == 0/*pops up only if caret is after first letter*/)))
            {
                Menu.Fragment = fragment;
                bool foundSelected = false;
                //build popup menu
                foreach (var item in sourceItems)
                {
                    item.Parent = Menu;
                    CompareResult res = item.Compare(text);
                    if(res != CompareResult.Hidden)
                        visibleItems.Add(item);
                    if (res == CompareResult.VisibleAndSelected && !foundSelected)
                    {
                        foundSelected = true;
                        FocussedItemIndex = visibleItems.Count - 1;
                    }
                }

                if (foundSelected)
                {
                    AdjustScroll();
                    DoSelectedVisible();
                }
            }

            //show popup menu
            if (Count > 0)
            {
                if (!Menu.Visible)
                {
                    CancelEventArgs args = new CancelEventArgs();
                    Menu.OnOpening(args);
                    if(!args.Cancel)
                        Menu.Show(tb, point);
                }
                else
                    Invalidate();
            }
            else
                Menu.Close();
        }

        void tb_SelectionChanged(object sender, EventArgs e)
        {
            /*
            FastColoredTextBox tb = sender as FastColoredTextBox;
            
            if (Math.Abs(prevSelection.iChar - tb.Selection.Start.iChar) > 1 ||
                        prevSelection.iLine != tb.Selection.Start.iLine)
                Menu.Close();
            prevSelection = tb.Selection.Start;*/
            if (Menu.Visible)
            {
                bool needClose = false;

                if (!tb.Selection.IsEmpty)
                    needClose = true;
                else
                    if (!Menu.Fragment.Contains(tb.Selection.Start))
                    {
                        if (tb.Selection.Start.iLine == Menu.Fragment.End.iLine && tb.Selection.Start.iChar == Menu.Fragment.End.iChar + 1)
                        {
                            //user press key at end of fragment
                            char c = tb.Selection.CharBeforeStart;
                            if (!Regex.IsMatch(c.ToString(), Menu.SearchPattern))//check char
                                needClose = true;
                        }
                        else
                            needClose = true;
                    }

                if (needClose)
                    Menu.Close();
            }
            
        }

        void tb_KeyDown(object sender, KeyEventArgs e)
        {
            var tb = sender as FastColoredTextBox;

            if (Menu.Visible)
                if (ProcessKey(e.KeyCode, e.Modifiers))
                    e.Handled = true;

            if (!Menu.Visible)
            {
                if (tb.HotkeysMapping.ContainsKey(e.KeyData) && tb.HotkeysMapping[e.KeyData] == FCTBAction.AutocompleteMenu)
                {
                    DoAutocomplete();
                    e.Handled = true;
                }
                else
                {
                    if (e.KeyCode == Keys.Escape && timer.Enabled)
                        timer.Stop();
                }
            }
        }

        void AdjustScroll()
        {
            if (oldItemCount == visibleItems.Count)
                return;

            int needHeight = ItemHeight * visibleItems.Count + 1;
            Height = Math.Min(needHeight, MaximumSize.Height);
            Menu.CalcSize();

            AutoScrollMinSize = new Size(0, needHeight);
            oldItemCount = visibleItems.Count;
        }

        override void OnPaint(PaintEventArgs e)
        {
            AdjustScroll();

            var itemHeight = ItemHeight;
            int startI = VerticalScroll.Value / itemHeight - 1;
            int finishI = (VerticalScroll.Value + ClientSize.Height) / itemHeight + 1;
            startI = Math.Max(startI, 0);
            finishI = Math.Min(finishI, visibleItems.Count);
            int y = 0;
            int leftPadding = 18;
            for (int i = startI; i < finishI; i++)
            {
                y = i * itemHeight - VerticalScroll.Value;

                var item = visibleItems[i];

                if(item.BackColor != Color.Transparent)
                using (var brush = new SolidBrush(item.BackColor))
                    e.Graphics.FillRectangle(brush, 1, y, ClientSize.Width - 1 - 1, itemHeight - 1);

                if (ImageList != null && visibleItems[i].ImageIndex >= 0)
                    e.Graphics.DrawImage(ImageList.Images[item.ImageIndex], 1, y);

                if (i == FocussedItemIndex)
                using (var selectedBrush = new LinearGradientBrush(new Point(0, y - 3), new Point(0, y + itemHeight), Color.Transparent, SelectedColor))
                using (var pen = new Pen(SelectedColor))
                {
                    e.Graphics.FillRectangle(selectedBrush, leftPadding, y, ClientSize.Width - 1 - leftPadding, itemHeight - 1);
                    e.Graphics.DrawRectangle(pen, leftPadding, y, ClientSize.Width - 1 - leftPadding, itemHeight - 1);
                }

                if (i == hoveredItemIndex)
                using(var pen = new Pen(HoveredColor))
                    e.Graphics.DrawRectangle(pen, leftPadding, y, ClientSize.Width - 1 - leftPadding, itemHeight - 1);

                using (var brush = new SolidBrush(item.ForeColor != Color.Transparent ? item.ForeColor : ForeColor))
                    e.Graphics.DrawString(item.ToString(), Font, brush, leftPadding, y);
            }
        }

        override void OnScroll(ScrollEventArgs se)
        {
            base.OnScroll(se);
            Invalidate();
        }

        override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                FocussedItemIndex = PointToItemIndex(e.Location);
                DoSelectedVisible();
                Invalidate();
            }
        }

        override void OnMouseDoubleClick(MouseEventArgs e)
        {
            base.OnMouseDoubleClick(e);
            FocussedItemIndex = PointToItemIndex(e.Location);
            Invalidate();
            OnSelecting();
        }

        internal virtual void OnSelecting()
        {
            if (FocussedItemIndex < 0 || FocussedItemIndex >= visibleItems.Count)
                return;
            tb.TextSource.Manager.BeginAutoUndoCommands();
            try
            {
                AutocompleteItem item = FocussedItem;
                SelectingEventArgs args = new SelectingEventArgs()
                {
                    Item = item,
                    SelectedIndex = FocussedItemIndex
                };

                Menu.OnSelecting(args);

                if (args.Cancel)
                {
                    FocussedItemIndex = args.SelectedIndex;
                    Invalidate();
                    return;
                }

                if (!args.Handled)
                {
                    var fragment = Menu.Fragment;
                    DoAutocomplete(item, fragment);
                }

                Menu.Close();
                //
                SelectedEventArgs args2 = new SelectedEventArgs()
                {
                    Item = item,
                    Tb = Menu.Fragment.tb
                };
                item.OnSelected(Menu, args2);
                Menu.OnSelected(args2);
            }
            finally
            {
                tb.TextSource.Manager.EndAutoUndoCommands();
            }
        }

        void DoAutocomplete(AutocompleteItem item, Range fragment)
        {
            string newText = item.GetTextForReplace();

            //replace text of fragment
            var tb = fragment.tb;

            tb.BeginAutoUndo();
            tb.TextSource.Manager.ExecuteCommand(new SelectCommand(tb.TextSource));
            if (tb.Selection.ColumnSelectionMode)
            {
                var start = tb.Selection.Start;
                var end = tb.Selection.End;
                start.iChar = fragment.Start.iChar;
                end.iChar = fragment.End.iChar;
                tb.Selection.Start = start;
                tb.Selection.End = end;
            }
            else
            {
                tb.Selection.Start = fragment.Start;
                tb.Selection.End = fragment.End;
            }
            tb.InsertText(newText);
            tb.TextSource.Manager.ExecuteCommand(new SelectCommand(tb.TextSource));
            tb.EndAutoUndo();
            tb.Focus();
        }

        int PointToItemIndex(Point p)
        {
            return (p.Y + VerticalScroll.Value) / ItemHeight;
        }

        override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            ProcessKey(keyData, Keys.None);
            
            return base.ProcessCmdKey(ref msg, keyData);
        }

        bool ProcessKey(Keys keyData, Keys keyModifiers)
        {
            if (keyModifiers == Keys.None)
            switch (keyData)
            {
                case Keys.Down:
                    SelectNext(+1);
                    return true;
                case Keys.PageDown:
                    SelectNext(+10);
                    return true;
                case Keys.Up:
                    SelectNext(-1);
                    return true;
                case Keys.PageUp:
                    SelectNext(-10);
                    return true;
                case Keys.Enter:
                    OnSelecting();
                    return true;
                case Keys.Tab:
                    if (!AllowTabKey)
                        break;
                    OnSelecting();
                    return true;
                case Keys.Escape:
                    Menu.Close();
                    return true;
            }

            return false;
        }

        void SelectNext(int shift)
        {
            FocussedItemIndex = Math.Max(0, Math.Min(FocussedItemIndex + shift, visibleItems.Count - 1));
            DoSelectedVisible();
            //
            Invalidate();
        }

        void DoSelectedVisible()
        {
            if (FocussedItem != null)
                SetToolTip(FocussedItem);

            var y = FocussedItemIndex * ItemHeight - VerticalScroll.Value;
            if (y < 0)
                VerticalScroll.Value = FocussedItemIndex * ItemHeight;
            if (y > ClientSize.Height - ItemHeight)
                VerticalScroll.Value = Math.Min(VerticalScroll.Maximum, FocussedItemIndex * ItemHeight - ClientSize.Height + ItemHeight);
            //some magic for update scrolls
            AutoScrollMinSize -= new Size(1, 0);
            AutoScrollMinSize += new Size(1, 0);
        }

        void SetToolTip(AutocompleteItem autocompleteItem)
        {
            var title = autocompleteItem.ToolTipTitle;
            var text = autocompleteItem.ToolTipText;

            if (string.IsNullOrEmpty(title))
            {
                toolTip.ToolTipTitle = null;
                toolTip.SetToolTip(this, null);
                return;
            }

            IWin32Window window = this->Parent ?? this;
            Point location = new Point((window == this ? Width : Right) + 3, 0);

            if (string.IsNullOrEmpty(text))
            {
                toolTip.ToolTipTitle = null;
                toolTip.Show(title, window, location.X, location.Y, ToolTipDuration);
            }
            else
            {
                toolTip.ToolTipTitle = title;
                toolTip.Show(text, window, location.X, location.Y, ToolTipDuration);
            }
        }

        int Count
        {
            get { return visibleItems.Count; }
        }

        void SetAutocompleteItems(ICollection<string> items)
        {
            List<AutocompleteItem> list = new List<AutocompleteItem>(items.Count);
            foreach (var item in items)
                list.Add(new AutocompleteItem(item));
            SetAutocompleteItems(list);
        }

        void SetAutocompleteItems(IEnumerable<AutocompleteItem> items)
        {
            sourceItems = items;
        }
    }

    struct SelectingEventArgs : EventArgs
    {
        AutocompleteItem Item { get; internal set; }
        bool Cancel {get;set;}
        int SelectedIndex{get;set;}
        bool Handled { get; set; }
    }

    struct SelectedEventArgs : EventArgs
    {
        AutocompleteItem Item { get; internal set; }
        FastColoredTextBox Tb { get; set; }
    }
}
