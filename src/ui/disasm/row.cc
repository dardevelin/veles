/*
 * Copyright 2018 CodiLime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "ui/disasm/row.h"
#include <iostream>

namespace veles {
namespace ui {
namespace disasm {

Row::Row() {
  layout_ = new QHBoxLayout();
  layout_->setSpacing(0);
  layout_->setMargin(0);

  text_ = new QLabel;
  comment_ = new QLabel;
  address_ = new QLabel;

  address_->setMaximumWidth(100);

  layout_->addWidget(address_);
  layout_->addWidget(text_);
  layout_->addWidget(comment_);

  setLayout(layout_);
}

void Row::setIndent(int level) { text_->setIndent(level * 20); }

template <typename T>
T* to(TextRepr* ptr) {
  return dynamic_cast<T*>(ptr);
}

template <typename T>
bool is(TextRepr* ptr) {
  return to<T>(ptr) != nullptr;
}

void Row::setEntry(const EntryChunkCollapsed* entry) {
  id_ = entry->chunk->id;
  address_->setText(
      QString("%1").arg(entry->chunk->addr_begin, 8, 16, QChar('0')));
  comment_->setText("; " + entry->chunk->comment);

  text_->setText(QString(entry->chunk->text_repr->string()));

  QString str;
  auto repr = entry->chunk->text_repr.get();
  if (is<Sublist>(repr)) {
    for (auto& chldPtr : to<Sublist>(repr)->children()) {
      if (chldPtr != nullptr) {
        if (is<Keyword>(chldPtr.get())) {
          auto keyword = to<Keyword>(chldPtr.get());
          if (keyword->keywordType() == KeywordType::REGISTER) {
            str.append("<font color='green'>");
            str.append(chldPtr->string());
            str.append("</font>");
          } else if (keyword->keywordType() == KeywordType::OPCODE) {
            str.append("<font color=#1ABC9C>");
            str.append(chldPtr->string());
            str.append("</font>");
          } else
            str.append(chldPtr->string());

        } else {
          str.append(chldPtr->string());
        }
      }
    }
    text_->setText(str);
  }
}
// reszta logiki todo

void Row::setEntry(const EntryChunkBegin* entry) {
  id_ = entry->chunk->id;
  address_->setText(
      QString("%1").arg(entry->chunk->addr_begin, 8, 16, QChar('0')));
  comment_->setText("; " + entry->chunk->comment);
  text_->setText(
      QString(entry->chunk->display_name + "::" + entry->chunk->type + " {"));
}

void Row::setEntry(const EntryChunkEnd* entry) {
  id_ = entry->chunk->id;
  address_->setText(
      QString("%1").arg(entry->chunk->addr_end, 8, 16, QChar('0')));
  text_->setText("}");
}

void Row::setEntry(const EntryOverlap* entry) {}

void Row::setEntry(const EntryField* entry) {}

void Row::mouseDoubleClickEvent(QMouseEvent* event) {
  emit chunkCollapse(this->id_);
}

void Row::toggleColumn(Row::ColumnName column_name) {
  switch (column_name) {
    case Row::ColumnName::Address:
      address_->setVisible(!address_->isVisible());
      break;
    case Row::ColumnName::Chunks:
      text_->setVisible(!text_->isVisible());
      break;
    case Row::ColumnName::Comments:
      comment_->setVisible(!comment_->isVisible());
      break;
    default:
      break;
  }
}

}  // namespace disasm
}  // namespace ui
}  // namespace veles
