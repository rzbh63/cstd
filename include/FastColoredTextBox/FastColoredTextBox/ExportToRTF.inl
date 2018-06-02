

{
  struct RTFStyleDescriptor
  {
    Color ForeColor;
    Color BackColor;
    string AdditionalTags;
  };
    /// <summary>
    /// Exports colored text as RTF
    /// </summary>
    /// <remarks>At this time only TextStyle renderer is supported. Other styles is not exported.</remarks>
    struct ExportToRTF
    {
        /// <summary>
        /// Includes line numbers
        /// </summary>
        bool IncludeLineNumbers;
        /// <summary>
        /// Use original font
        /// </summary>
        bool UseOriginalFont;

        FastColoredTextBox* tb;
        Dictionary<Color, int> colorTable;

        ExportToRTF()
        {
            UseOriginalFont = true;
        }

        string GetRtf(FastColoredTextBox* tb)
        {
            this->tb = tb;
            Range sel(tb);
            sel.SelectAll();
            return GetRtf(sel);
        }

        string GetRtf(Range r)
        {
            this->tb = r.tb;
            Dictionary<StyleIndex, void*> styles;
            StringBuilder sb;
            StringBuilder tempSB;
            StyleIndex currentStyleId = StyleIndex_None;
            r.Normalize();
            int currentLine = r.Start.iLine;
            styles[currentStyleId] = null;
            colorTable.Clear();
            //
            var lineNumberColor = GetColorTableNumber(r.tb.LineNumberColor);

            if (IncludeLineNumbers) {
                tempSB.AppendFormat("{{\cf{1} {0}}}\tab", currentLine + 1, lineNumberColor);
            }
            //
            foreach (Place p in r)
            {
                Char c = r.tb[p.iLine][p.iChar];
                if (c.style != currentStyleId)
                {
                    Flush(sb, tempSB, currentStyleId);
                    currentStyleId = c.style;
                    styles[currentStyleId] = null;
                }

                if (p.iLine != currentLine)
                {
                    for (int i = currentLine; i < p.iLine; i++)
                    {
                        tempSB.AppendLine("\line");
                        if (IncludeLineNumbers)
                            tempSB.AppendFormat("{{\cf{1} {0}}}\tab", i + 2, lineNumberColor);
                    }
                    currentLine = p.iLine;
                }
                switch (c.c)
                {
                    case '\\':
                        tempSB.Append("\\");
                        break;
                    case '{':
                        tempSB.Append("\{");
                        break;
                    case '}':
                        tempSB.Append("\}");
                        break;
                    default:
                        var ch = c.c;
                        var code = (int)ch;
                        if(code < 128)
                            tempSB.Append(c.c);
                        else
                            tempSB.AppendFormat("{{\u{0}}}", code);
                        break;
                }
            }
            Flush(sb, tempSB, currentStyleId);
           
            //build color table
            var list = new SortedList<int, Color>();
            foreach (var pair in colorTable)
                list.Add(pair.Value, pair.Key);

            tempSB.Length = 0;
            tempSB.AppendFormat("{{\colortbl;");

            foreach (var pair in list)
                tempSB.Append(GetColorAsString(pair.Value)+";");
            tempSB.AppendLine("}");

            //
            if (UseOriginalFont)
            {
                sb.Insert(0, string.Format("{{\fonttbl{{\f0\fmodern {0};}}}}{{\fs{1} ",
                                tb.Font.Name, (int)(2 * tb.Font.SizeInPoints), tb.CharHeight));
                sb.AppendLine("}");
            }

            sb.Insert(0, tempSB.ToString());

            sb.Insert(0, "{\rtf1\ud\deff0");
            sb.AppendLine("}");

            return sb.ToString();
        }

        RTFStyleDescriptor GetRtfDescriptor(StyleIndex styleIndex)
        {
            List<Style> styles = new List<Style>();
            //find text renderer
            TextStyle textStyle = null;
            int mask = 1;
            bool hasTextStyle = false;
            for (int i = 0; i < tb.Styles.Length; i++)
            {
              if (tb.Styles[i] != null && ((int)styleIndex & mask) != 0) {
                    if (tb.Styles[i].IsExportable)
                    {
                        var style = tb.Styles[i];
                        styles.Add(style);

                        bool isTextStyle = style is TextStyle;
                        if (isTextStyle)
                            if (!hasTextStyle || tb.AllowSeveralTextStyleDrawing)
                            {
                                hasTextStyle = true;
                                textStyle = style as TextStyle;
                            }
                    }
              }
              mask = mask << 1;
            }
            //add TextStyle css
            RTFStyleDescriptor result = null;

            if (!hasTextStyle)
            {
                //draw by default renderer
                result = tb.DefaultStyle.GetRTF();
            }
            else
            {
                result = textStyle.GetRTF();
            }

            return result;
        }

        static string GetColorAsString(Color color)
        {
            if (color == Color.Transparent)
                return "";
            return string.Format("\red{0}\green{1}\blue{2}", color.R, color.G, color.B);
        }

        void Flush(StringBuilder sb, StringBuilder tempSB, StyleIndex currentStyle)
        {
            //find textRenderer
            if (tempSB.Length == 0)
                return;

            var desc = GetRtfDescriptor(currentStyle);
            var cf = GetColorTableNumber(desc.ForeColor);
            var cb = GetColorTableNumber(desc.BackColor);
            var tags = new StringBuilder();
            if (cf >= 0)
                tags.AppendFormat("\cf{0}", cf);
            if (cb >= 0)
                tags.AppendFormat("\highlight{0}", cb);
            if(!string.IsNullOrEmpty(desc.AdditionalTags))
                tags.Append(desc.AdditionalTags.Trim());

            if(tags.Length > 0)
                sb.AppendFormat("{{{0} {1}}}", tags, tempSB.ToString());
            else
                sb.Append(tempSB.ToString());
            tempSB.Length = 0;
        }

        int GetColorTableNumber(Color color)
        {
            if (color.A == 0)
                return -1;

            if (!colorTable.ContainsKey(color))
                colorTable[color] = colorTable.Count + 1;

            return colorTable[color];
        }
    };

}
