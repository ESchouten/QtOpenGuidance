/* Copyright (c) 2012, STANISLAW ADASZEWSKI
 * Copyright (c) 2020, Christian Riggenbach
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of STANISLAW ADASZEWSKI nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <QObject>
#include <QGraphicsPathItem>

class QNEPort;

class QNEConnection : public QGraphicsPathItem {
  public:
    enum { Type = QGraphicsItem::UserType + 2 };

    QNEConnection( QGraphicsItem* parent = nullptr );
    ~QNEConnection();

    void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget ) override;

    void setPos1( QPointF p );
    void setPos2( QPointF p );

    void setPort1( QNEPort* p );
    bool setPort2( QNEPort* p );
    void updatePosFromPorts();
    void updatePath();
    QNEPort* port1() const;
    QNEPort* port2() const;

    int type() const override {
      return Type;
    }

    void toJSON( QJsonObject& json );

  private:
    QPointF pos1;
    QPointF pos2;
    QNEPort* m_port1 = nullptr;
    QNEPort* m_port2 = nullptr;

    QMetaObject::Connection connection;
};

