// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CGUIFileOpenDialog.h"
#ifdef _IRR_COMPILE_WITH_GUI_
//#include <locale.h>
//#include "IGUISkin.h"
//#include "IGUIEnvironment.h"
//#include "IVideoDriver.h"
//#include "IGUIButton.h"
//#include "IGUIStaticText.h"
//#include "IGUIFont.h"
//#include "IGUIFontBitmap.h"
//#include "IFileList.h"
//#include "os.h"
const s32 FOD_WIDTH = 350;
const s32 FOD_HEIGHT = 250;
//! constructor
CGUIFileOpenDialog::CGUIFileOpenDialog(const char* title,
    IGUIEnvironment* environment, IGUIElement* parent, s32 id,
    bool restoreCWD, char*::char_type* startDir)
  : IGUIFileOpenDialog(environment, parent, id,
      IRECT((parent->getAbsolutePosition().getWidth() - FOD_WIDTH) / 2,
          (parent->getAbsolutePosition().getHeight() - FOD_HEIGHT) / 2,
          (parent->getAbsolutePosition().getWidth() - FOD_WIDTH) / 2 + FOD_WIDTH,
          (parent->getAbsolutePosition().getHeight() - FOD_HEIGHT) / 2 + FOD_HEIGHT)),
  FileNameText(0), FileList(0), Dragging(false)
{
#ifdef _DEBUG
  IGUIElement::setDebugName("CGUIFileOpenDialog");
#endif
  Text = title;
  FileSystem = Environment ? Environment->getFileSystem() : 0;
  if (FileSystem) {
    FileSystem->grab();
    if (restoreCWD) {
      RestoreDirectory = FileSystem->getWorkingDirectory();
    }
    if (startDir) {
      StartDirectory = startDir;
      FileSystem->changeWorkingDirectoryTo(startDir);
    }
  }
  else {
    return;
  }
  IGUISpriteBank* sprites = 0;
  SColor color(255, 255, 255, 255);
  IGUISkin* skin = Environment->getSkin();
  if (skin) {
    sprites = skin->getSpriteBank();
    color = skin->getColor(EGDC_WINDOW_SYMBOL);
  }
  const s32 buttonw = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
  const s32 posx = RelativeRect.getWidth() - buttonw - 4;
  CloseButton = Environment->addButton(IRECT(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
      "", skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : "Close");
  CloseButton->setSubElement(true);
  CloseButton->setTabStop(false);
  if (sprites) {
    CloseButton->setSpriteBank(sprites);
    CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), color);
    CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), color);
  }
  CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
  CloseButton->grab();
  OKButton = Environment->addButton(
      IRECT(RelativeRect.getWidth() - 80, 30, RelativeRect.getWidth() - 10, 50),
      this, -1, skin ? skin->getDefaultText(EGDT_MSG_BOX_OK) : "OK");
  OKButton->setSubElement(true);
  OKButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
  OKButton->grab();
  CancelButton = Environment->addButton(
      IRECT(RelativeRect.getWidth() - 80, 55, RelativeRect.getWidth() - 10, 75),
      this, -1, skin ? skin->getDefaultText(EGDT_MSG_BOX_CANCEL) : "Cancel");
  CancelButton->setSubElement(true);
  CancelButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
  CancelButton->grab();
  FileBox = Environment->addListBox(IRECT(10, 55, RelativeRect.getWidth() - 90, 230), this, -1, true);
  FileBox->setSubElement(true);
  FileBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
  FileBox->grab();
  FileNameText = Environment->addEditBox(0, IRECT(10, 30, RelativeRect.getWidth() - 90, 50), true, this);
  FileNameText->setSubElement(true);
  FileNameText->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
  FileNameText->grab();
  setTabGroup(true);
  fillListBox();
}
//! destructor
CGUIFileOpenDialog::~CGUIFileOpenDialog()
{
  if (CloseButton) {
    CloseButton->drop();
  }
  if (OKButton) {
    OKButton->drop();
  }
  if (CancelButton) {
    CancelButton->drop();
  }
  if (FileBox) {
    FileBox->drop();
  }
  if (FileNameText) {
    FileNameText->drop();
  }
  if (FileSystem) {
    // revert to original CWD if path was set in constructor
    if (RestoreDirectory_size) {
      FileSystem->changeWorkingDirectoryTo(RestoreDirectory);
    }
    FileSystem->drop();
  }
  if (FileList) {
    FileList->drop();
  }
}
//! returns the filename of the selected file. Returns NULL, if no file was selected.
const char* CGUIFileOpenDialog::getFileName() const
{
  return FileName;
}
//! Returns the directory of the selected file. Returns NULL, if no directory was selected.
const char* CGUIFileOpenDialog::getDirectoryName()
{
  FileSystem->flattenFilename(FileDirectory);
  return FileDirectory;
}
//! called if an event happened.
bool CGUIFileOpenDialog::OnEvent(const SEvent& event)
{
  if (isEnabled()) {
    switch (event.EventType) {
    case EET_GUI_EVENT:
      switch (event.GUIEvent.EventType) {
      case EGET_ELEMENT_FOCUS_LOST:
        Dragging = false;
        break;
      case EGET_BUTTON_CLICKED:
        if (event.GUIEvent.Caller == CloseButton ||
            event.GUIEvent.Caller == CancelButton) {
          sendCancelEvent();
          remove();
          return true;
        }
        else if (event.GUIEvent.Caller == OKButton) {
          if (FileDirectory != "") {
            sendSelectedEvent(EGET_DIRECTORY_SELECTED);
          }
          if (FileName != "") {
            sendSelectedEvent(EGET_FILE_SELECTED);
            remove();
            return true;
          }
        }
        break;
      case EGET_LISTBOX_CHANGED: {
        s32 selected = FileBox->getSelected();
        if (FileList && FileSystem) {
          if (FileList->isDirectory(selected)) {
            FileName = "";
            FileDirectory = FileList->getFullFileName(selected);
          }
          else {
            FileDirectory = "";
            FileName = FileList->getFullFileName(selected);
          }
          return true;
        }
      }
      break;
      case EGET_LISTBOX_SELECTED_AGAIN: {
        const s32 selected = FileBox->getSelected();
        if (FileList && FileSystem) {
          if (FileList->isDirectory(selected)) {
            FileDirectory = FileList->getFullFileName(selected);
            FileSystem->changeWorkingDirectoryTo(FileList->getFileName(selected));
            fillListBox();
            FileName = "";
          }
          else {
            FileName = FileList->getFullFileName(selected);
          }
          return true;
        }
      }
      break;
      case EGET_EDITBOX_ENTER:
        if (event.GUIEvent.Caller == FileNameText) {
          char* dir(FileNameText->getText());
          if (FileSystem->changeWorkingDirectoryTo(dir)) {
            fillListBox();
            FileName = "";
          }
          return true;
        }
        break;
      default:
        break;
      }
      break;
    case EET_MOUSE_INPUT_EVENT:
      switch (event.MouseInput.Event) {
      case EMIE_MOUSE_WHEEL:
        return FileBox->OnEvent(event);
      case EMIE_LMOUSE_PRESSED_DOWN:
        DragStart.X = event.MouseInput.X;
        DragStart.Y = event.MouseInput.Y;
        Dragging = true;
        Environment->setFocus(this);
        return true;
      case EMIE_LMOUSE_LEFT_UP:
        Dragging = false;
        return true;
      case EMIE_MOUSE_MOVED:
        if (!event.MouseInput.isLeftPressed()) {
          Dragging = false;
        }
        if (Dragging) {
          // gui window should not be dragged outside its parent
          if (Parent)
            if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X + 1 ||
                event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y + 1 ||
                event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X - 1 ||
                event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y - 1) {
              return true;
            }
          move(position2di(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
          DragStart.X = event.MouseInput.X;
          DragStart.Y = event.MouseInput.Y;
          return true;
        }
        break;
      default:
        break;
      }
    default:
      break;
    }
  }
  return IGUIElement::OnEvent(event);
}
//! draws the element and its children
void CGUIFileOpenDialog::draw()
{
  if (!IsVisible) {
    return;
  }
  IGUISkin* skin = Environment->getSkin();
  IRECT rect = AbsoluteRect;
  rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER),
      rect, &AbsoluteClippingRect);
  if (Text_size) {
    rect.UpperLeftCorner.X += 2;
    rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;
    IGUIFont* font = skin->getFont(EGDF_WINDOW);
    if (font)
      font->draw(Text, rect,
          skin->getColor(EGDC_ACTIVE_CAPTION),
          false, true, &AbsoluteClippingRect);
  }
  IGUIElement::draw();
}
//! Writes attributes of the element.
/* Not sure if this will really work out properly. Saving paths can be
rather problematic. */
void CGUIFileOpenDialog::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
{
  IGUIFileOpenDialog::serializeAttributes(out, options);
  out->addString("StartDirectory", StartDirectory);
  out->addBool("RestoreDirectory", (RestoreDirectory_size != 0));
}
//! Reads attributes of the element
/* Note that thiese paths changes will happen at arbitrary places upon
load of the gui description. This may be undesired. */
void CGUIFileOpenDialog::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
{
  StartDirectory = in->getAttributeAsString("StartDirectory");
  const bool restore = in->getAttributeAsBool("RestoreDirectory");
  if (restore) {
    RestoreDirectory = FileSystem->getWorkingDirectory();
  }
  else {
    RestoreDirectory = "";
  }
  if (StartDirectory_size) {
    FileSystem->changeWorkingDirectoryTo(StartDirectory);
  }
  IGUIFileOpenDialog::deserializeAttributes(in, options);
}
//! fills the listbox with files.
void CGUIFileOpenDialog::fillListBox()
{
  IGUISkin* skin = Environment->getSkin();
  if (!FileSystem || !FileBox || !skin) {
    return;
  }
  if (FileList) {
    FileList->drop();
  }
  FileBox->clear();
  FileList = FileSystem->createFileList();
  char* s;
  if (FileList) {
    int i;
    for (i = 0; i < FileList->getFileCount(); ++i) {
      s = FileList->getFileName(i);
      FileBox->addItem(s, skin->getIcon(FileList->isDirectory(i) ? EGDI_DIRECTORY : EGDI_FILE));
    }
  }
  if (FileNameText) {
    s = FileSystem->getWorkingDirectory();
    FileDirectory = s;
    FileNameText->setText(s);
  }
}
//! sends the event that the file has been selected.
void CGUIFileOpenDialog::sendSelectedEvent(EGUI_EVENT_TYPE type)
{
  SEvent event;
  event.EventType = EET_GUI_EVENT;
  event.GUIEvent.Caller = this;
  event.GUIEvent.Element = 0;
  event.GUIEvent.EventType = type;
  Parent->OnEvent(event);
}
//! sends the event that the file choose process has been canceld
void CGUIFileOpenDialog::sendCancelEvent()
{
  SEvent event;
  event.EventType = EET_GUI_EVENT;
  event.GUIEvent.Caller = this;
  event.GUIEvent.Element = 0;
  event.GUIEvent.EventType = EGET_FILE_CHOOSE_DIALOG_CANCELLED;
  Parent->OnEvent(event);
}
#endif // _IRR_COMPILE_WITH_GUI_

