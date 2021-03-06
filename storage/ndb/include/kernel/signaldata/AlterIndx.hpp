/* Copyright (c) 2003, 2005 MySQL AB
   Use is subject to license terms

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA */

#ifndef ALTER_INDX_HPP
#define ALTER_INDX_HPP

#include "SignalData.hpp"
#include <Bitmask.hpp>
#include <trigger_definitions.h>

/**
 * AlterIndxReq.
 */
class AlterIndxReq {
  friend bool printALTER_INDX_REQ(FILE*, const Uint32*, Uint32, Uint16);

public:
  enum RequestType {
    RT_UNDEFINED = 0,
    RT_USER = 1,
    RT_CREATE_INDEX = 2,
    RT_DROP_INDEX = 3,
    RT_SYSTEMRESTART = 4,
    RT_NODERESTART = 5,
    RT_DICT_PREPARE = 1 << 4,
    RT_DICT_TC = 5 << 4,
    RT_DICT_COMMIT = 0xC << 4,
    RT_DICT_ABORT = 0xF << 4,
    RT_TC = 5 << 8,
    RT_TUX = 8 << 8
  };
  STATIC_CONST( SignalLength = 7 );

private:
  Uint32 m_userRef;
  Uint32 m_connectionPtr;
  Uint32 m_requestInfo;
  Uint32 m_tableId;
  Uint32 m_indexId;             // only set by DICT
  Uint32 m_indexVersion;
  Uint32 m_online;              // new state 0-offline 1-online
  // extra
  Uint32 m_opKey;

public:
  Uint32 getUserRef() const {
    return m_userRef;
  }
  void setUserRef(Uint32 val) {
    m_userRef = val;
  }
  Uint32 getConnectionPtr() const {
    return m_connectionPtr;
  }
  void setConnectionPtr(Uint32 val) {
    m_connectionPtr = val;
  }
  AlterIndxReq::RequestType getRequestType() const {
    const Uint32 val = BitmaskImpl::getField(1, &m_requestInfo, 0, 16);
    return (AlterIndxReq::RequestType)val;
  }
  void setRequestType(AlterIndxReq::RequestType val) {
    m_requestInfo = (Uint32)val;
  }
  Uint32 getRequestFlag() const {
    return BitmaskImpl::getField(1, &m_requestInfo, 16, 16);
  };
  void addRequestFlag(Uint32 val) {
    val |= BitmaskImpl::getField(1, &m_requestInfo, 16, 16);
    BitmaskImpl::setField(1, &m_requestInfo, 16, 16, val);
  };
  Uint32 getTableId() const {
    return m_tableId;
  }
  void setTableId(Uint32 val) {
    m_tableId = val;
  }
  Uint32 getIndexId() const {
    return m_indexId;
  }
  void setIndexId(Uint32 val) {
    m_indexId = val;
  }
  Uint32 getIndexVersion() const {
    return m_indexVersion;
  }
  void setIndexVersion(Uint32 val) {
    m_indexVersion = val;
  }
  Uint32 getOnline() const {
    return m_online;
  }
  void setOnline(Uint32 val) {
    m_online = val;
  }
  Uint32 getOpKey() const {
    return m_opKey;
  }
  void setOpKey(Uint32 val) {
    m_opKey = val;
  }
};

/**
 * AlterIndxConf.
 */
class AlterIndxConf {
  friend bool printALTER_INDX_CONF(FILE*, const Uint32*, Uint32, Uint16);

public:
  STATIC_CONST( InternalLength = 3 );
  STATIC_CONST( SignalLength = 6 );

private:
  Uint32 m_userRef;
  Uint32 m_connectionPtr;
  Uint32 m_requestInfo;
  Uint32 m_tableId;
  Uint32 m_indexId;
  Uint32 m_indexVersion;

public:
  Uint32 getUserRef() const {
    return m_userRef;
  }
  void setUserRef(Uint32 val) {
    m_userRef = val;
  }
  Uint32 getConnectionPtr() const {
    return m_connectionPtr;
  }
  void setConnectionPtr(Uint32 val) {
    m_connectionPtr = val;
  }
  AlterIndxReq::RequestType getRequestType() const {
    return (AlterIndxReq::RequestType)m_requestInfo;
  }
  void setRequestType(AlterIndxReq::RequestType val) {
    m_requestInfo = (Uint32)val;
  }
  Uint32 getTableId() const {
    return m_tableId;
  }
  void setTableId(Uint32 val) {
    m_tableId = val;
  }
  Uint32 getIndexId() const {
    return m_indexId;
  }
  void setIndexId(Uint32 val) {
    m_indexId = val;
  }
  Uint32 getIndexVersion() const {
    return m_indexVersion;
  }
  void setIndexVersion(Uint32 val) {
    m_indexVersion = val;
  }
};

/**
 * AlterIndxRef.
 */
class AlterIndxRef {
  friend bool printALTER_INDX_REF(FILE*, const Uint32*, Uint32, Uint16);

public:
  enum ErrorCode {
    NoError = 0,
    Busy = 701,
    NotMaster = 702,
    IndexNotFound = 4243,
    IndexExists = 4244,
    BadRequestType = 4247,
    NotAnIndex = 4254,
    BadState = 4347,
    Inconsistency = 4348
  };
  STATIC_CONST( SignalLength = AlterIndxConf::SignalLength + 3 );

private:
  AlterIndxConf m_conf;
  //Uint32 m_userRef;
  //Uint32 m_connectionPtr;
  //Uint32 m_requestInfo;
  //Uint32 m_tableId;
  //Uint32 m_indexId;
  //Uint32 m_indexVersion;
  Uint32 m_errorCode;
  Uint32 m_errorLine;
  union {
    Uint32 m_errorNode;
    Uint32 masterNodeId; // if NotMaster
  };
public:
  AlterIndxConf* getConf() {
    return &m_conf;
  }
  const AlterIndxConf* getConf() const {
    return &m_conf;
  }
  Uint32 getUserRef() const {
    return m_conf.getUserRef();
  }
  void setUserRef(Uint32 val) {
    m_conf.setUserRef(val);
  }
  Uint32 getConnectionPtr() const {
    return m_conf.getConnectionPtr();
  }
  void setConnectionPtr(Uint32 val) {
    m_conf.setConnectionPtr(val);
  }
  AlterIndxReq::RequestType getRequestType() const {
    return m_conf.getRequestType();
  }
  void setRequestType(AlterIndxReq::RequestType val) {
    m_conf.setRequestType(val);
  }
  Uint32 getTableId() const {
    return m_conf.getTableId();
  }
  void setTableId(Uint32 val) {
    m_conf.setTableId(val);
  }
  Uint32 getIndexId() const {
    return m_conf.getIndexId();
  }
  void setIndexId(Uint32 val) {
    m_conf.setIndexId(val);
  }
  Uint32 getIndexVersion() const {
    return m_conf.getIndexVersion();
  }
  void setIndexVersion(Uint32 val) {
    m_conf.setIndexVersion(val);
  }
  AlterIndxRef::ErrorCode getErrorCode() const {
    return (AlterIndxRef::ErrorCode)m_errorCode;
  }
  void setErrorCode(AlterIndxRef::ErrorCode val) {
    m_errorCode = (Uint32)val;
  }
  Uint32 getErrorLine() const {
    return m_errorLine;
  }
  void setErrorLine(Uint32 val) {
    m_errorLine = val;
  }
  Uint32 getErrorNode() const {
    return m_errorNode;
  }
  void setErrorNode(Uint32 val) {
    m_errorNode = val;
  }
};

#endif
