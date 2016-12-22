#include "customtextedit.h"

CustomTextEdit::CustomTextEdit(QWidget *parent)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(handleTextChanged()));
}

void CustomTextEdit::handleTextChanged()
{
    emit customTextChanged(this->objectName(), this->textCursor().selectedText());
}
