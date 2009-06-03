/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * Storage.cpp - Redland C++ Storage classes
 *
 * Copyright (C) 2008, David Beckett http://www.dajobe.org/
 *
 * This package is Free Software and part of Redland http://librdf.org/
 *
 * It is licensed under the following three licenses as alternatives:
 *   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
 *   2. GNU General Public License (GPL) V2 or any newer version
 *   3. Apache License, V2.0 or any newer version
 *
 * You may not use this file except in compliance with at least one of
 * the above three licenses.
 *
 * See LICENSE.html or LICENSE.txt at the top of this package for the
 * complete terms and further detail along with the license texts for
 * the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
 *
 *
 */


#ifdef HAVE_CONFIG_H
#include <redlandpp_config.h>
#endif

#include <iostream>


#include <redland.h>
#include <rdf_storage.h>
#include <rdf_storage_module.h>
#include "redlandpp/Exception.hpp"
#include "redlandpp/CLuceneStorage.hpp"
#include "redlandpp/World.hpp"
#include "redlandpp/Wrapper.hpp"
#include "redlandpp/rdf_storage_clucene.hpp"
//#include <CLucene/document/FieldSelector.h>


namespace Redland {

using namespace std;
using namespace lucene::document;
using namespace lucene::index;
using namespace lucene::store;
using namespace lucene::search;


// ================ TermDocs stream ============
typedef struct{
  Term* term;
  TermDocs* termDocs;
  CLuceneStorageImpl* storage;
  librdf_statement* currentStatement;
  char textBuffer[CLuceneStorageImpl::documentNumberUriSize + 11];
} librdf_storage_clucene_termdocs_stream_data;

static void
librdf_storage_clucene_termdocs_stream_init(librdf_storage_clucene_termdocs_stream_data* data, CLuceneStorageImpl* storage, librdf_statement* currentStatement){
    data->term = NULL;
    data->termDocs = NULL;
    data->storage = storage;
    data->currentStatement = currentStatement;
    *data->textBuffer = 0;
}
static void
librdf_storage_clucene_termdocs_stream_free(void* context){
  librdf_storage_clucene_termdocs_stream_data* stream=(librdf_storage_clucene_termdocs_stream_data*)context;

  _CLDECDELETE(stream->term);
  _CLDELETE(stream->termDocs);
  if ( stream->currentStatement ) librdf_free_statement(stream->currentStatement);
  free(stream);
}
static int
librdf_storage_clucene_termdocs_stream_is_end(void* context){
  librdf_storage_clucene_termdocs_stream_data* stream=(librdf_storage_clucene_termdocs_stream_data*)context;

  return stream->termDocs == NULL  < 0 ? 1 : 0;
}
static int
librdf_storage_clucene_termdocs_stream_goto_next(void* context){
  librdf_storage_clucene_termdocs_stream_data* stream=(librdf_storage_clucene_termdocs_stream_data*)context;

  if ( stream->termDocs->next() ){
    sprintf(stream->textBuffer + CLuceneStorageImpl::documentNumberUriSize, "%d", stream->termDocs->doc());
    librdf_statement_set_subject(stream->currentStatement,
      librdf_new_node_from_uri_string(stream->storage->world, (const unsigned char*)stream->textBuffer));

    return 0;
  }
  _CLDELETE( stream->termDocs );
  return 1;
}

static void*
librdf_storage_clucene_termdocs_stream_get_statement(void* context, int flags)
{
  librdf_storage_clucene_termdocs_stream_data* stream=(librdf_storage_clucene_termdocs_stream_data*)context;

  switch(flags) {
    case LIBRDF_ITERATOR_GET_METHOD_GET_OBJECT:


  printf("===========RETURNING==========\n");
if ( librdf_statement_get_subject(stream->currentStatement) )
  printf("subject=%s\n", (char*)librdf_node_to_string(librdf_statement_get_subject(stream->currentStatement)));
if ( librdf_statement_get_predicate(stream->currentStatement) )
  printf("predicate=%s\n", (char*)librdf_node_to_string(librdf_statement_get_predicate(stream->currentStatement)));
if ( librdf_statement_get_object(stream->currentStatement) )
  printf("object=%s\n", (char*)librdf_node_to_string(librdf_statement_get_object(stream->currentStatement)));

      return stream->currentStatement;
    case LIBRDF_ITERATOR_GET_METHOD_GET_CONTEXT:
      assert(false);
    default:
      abort();
  }
}

// ================ Single Doc stream ============
typedef struct{
  librdf_statement* currentStatement;
} librdf_storage_clucene_singledoc_stream_data;

static void
librdf_storage_clucene_singledoc_stream_init(librdf_storage_clucene_singledoc_stream_data* data, librdf_statement* currentStatement){
    data->currentStatement = currentStatement;
}
static void
librdf_storage_clucene_singledoc_stream_free(void* context){
  librdf_storage_clucene_singledoc_stream_data* stream=(librdf_storage_clucene_singledoc_stream_data*)context;

  if ( stream->currentStatement ) librdf_free_statement(stream->currentStatement);
  free(stream);
}
static int
librdf_storage_clucene_singledoc_stream_is_end(void* context){
  librdf_storage_clucene_singledoc_stream_data* stream=(librdf_storage_clucene_singledoc_stream_data*)context;

  return stream->currentStatement == NULL ? 1 : 0;
}
static int
librdf_storage_clucene_singledoc_stream_goto_next(void* context){
  librdf_storage_clucene_singledoc_stream_data* stream=(librdf_storage_clucene_singledoc_stream_data*)context;
  //only one doc available...

  if ( stream->currentStatement ) librdf_free_statement(stream->currentStatement);
  return 1;
}

static void*
librdf_storage_clucene_singledoc_stream_get_statement(void* context, int flags)
{
  librdf_storage_clucene_singledoc_stream_data* stream=(librdf_storage_clucene_singledoc_stream_data*)context;

  switch(flags) {
    case LIBRDF_ITERATOR_GET_METHOD_GET_OBJECT:
  printf("===========RETURNING==========\n");
if ( librdf_statement_get_subject(stream->currentStatement) )
  printf("subject=%s\n", (char*)librdf_node_to_string(librdf_statement_get_subject(stream->currentStatement)));
if ( librdf_statement_get_predicate(stream->currentStatement) )
  printf("predicate=%s\n", (char*)librdf_node_to_string(librdf_statement_get_predicate(stream->currentStatement)));
if ( librdf_statement_get_object(stream->currentStatement) )
  printf("object=%s\n", (char*)librdf_node_to_string(librdf_statement_get_object(stream->currentStatement)));

      return stream->currentStatement;
    case LIBRDF_ITERATOR_GET_METHOD_GET_CONTEXT:
      assert(false);
    default:
      abort();
  }
}

// ================ CLuceneStorage class ============

CLuceneStorage::CLuceneStorage(World* w, const string& n, const string& opts)
  throw(Exception)
  : Storage(w, "clucene", n, opts)
{
}
CLuceneStorage::CLuceneStorage(World& w, const string& n, const string& opts)
  throw(Exception)
  : Storage(&w, "clucene", n, opts)
{
}
CLuceneStorage::~CLuceneStorage()
{
}


// ================ CLuceneStorageImpl class ============

const std::string CLuceneStorageImpl::documentNumberUri = "http://clucene.sf.net/document#";

CLuceneStorageImpl::CLuceneStorageImpl(librdf_storage* storage, librdf_world* world_):
  Redland::Wrapper<librdf_storage>((redland_object_free*)librdf_free_storage, storage),
  directory(NULL),
  world(world_)
{
}
/*
CLuceneStorageImpl::CLuceneStorageImpl(const CLuceneStorageImpl& clone)
  Redland::Wrapper<librdf_storage>((redland_object_free*)librdf_free_storage, storage),
{
}*/
const TCHAR* CLuceneStorageImpl::idFieldName(){
  return _T("http://freedesktop.org/standards/xesam/1.0/core#url");
}
bool CLuceneStorageImpl::setStatementObject(const TCHAR* fromField, int32_t fromDocNum, librdf_statement* statement){

  //TODO: MapFieldSelector map;
  //map.add(field,FieldSelector::LOAD);

  Document* doc = reader->document(fromDocNum);
  if ( doc ){
    librdf_statement_set_object(statement,
      librdf_new_node_from_literal( this->world, (const unsigned char*)StringToChar(doc->get(CLuceneStorageImpl::idFieldName())).c_str(),
      NULL, 0) );
    _CLDELETE(doc);
    return true;
  }
  return false;
}
int CLuceneStorageImpl::Init(const std::string& name, librdf_hash* options){

  char* _dir = librdf_hash_get_del(options, "dir");
  if ( _dir == NULL ){
      fprintf(stderr, "CLucene dir wasn't specified\n");
      return -1;
  }
  string dir = _dir;
  free(_dir);

  try{
    this->directory = FSDirectory::getDirectory(dir.c_str(), false);
    return 0;
  }catch(CLuceneError& err){
    fprintf(stderr, "CLucene Error: %s\n", err.what());
  }
  return -1;
}

CLuceneStorageImpl::~CLuceneStorageImpl()
{
}

void CLuceneStorageImpl::ensureOpen(){
  if ( reader == NULL ){
    reader = IndexReader::open(directory);
  }
}

int CLuceneStorageImpl::Close(){
  return 0;
}
int CLuceneStorageImpl::Open(librdf_model* model){
  return 0;
}

int CLuceneStorageImpl::Size(){
  ensureOpen();
  //TODO: this is not correct because rdf is asking for triplets, not documents...
  //go through each field and pull out how many terms are in it and add them all up...
  return reader->numDocs();
}

librdf_stream* CLuceneStorageImpl::FindStatementsWithOptions(
  librdf_statement* statement,
  librdf_node* context_node,
  librdf_hash* options){
  ensureOpen();

  librdf_node *subject, *predicate, *object;
  bool is_literal_match = false;
  string where;
  string joins;

  if ( options )
    is_literal_match = librdf_hash_get_as_boolean(options, "match-substring");

  if(statement) {
    subject=librdf_statement_get_subject(statement);
    predicate=librdf_statement_get_predicate(statement);
    object=librdf_statement_get_object(statement);
  }

  printf("===========FindStatementsWithOptions==========\n");
  if ( subject )
    printf("subject=%s\n", NodeToString(subject).c_str());
  if ( predicate )
    printf("predicate=%s\n", NodeToString(predicate).c_str());
  if ( object )
    printf("object=%s\n", NodeToString(object).c_str());
  printf("\n");

  if (predicate && object && !subject){
    //simple term query...
    librdf_storage_clucene_termdocs_stream_data* streamData;
    streamData = (librdf_storage_clucene_termdocs_stream_data*)malloc(sizeof(librdf_storage_clucene_termdocs_stream_data));
    librdf_storage_clucene_termdocs_stream_init(streamData, this, librdf_new_statement_from_statement(statement));

    streamData->term = _CLNEW Term(
      NodeToKeyword(predicate).c_str(),
      NodeToKeyword(object).c_str() );
    streamData->termDocs = reader->termDocs(streamData->term);

    strcpy(streamData->textBuffer, documentNumberUri.c_str());
    assert ( documentNumberUri.length() == documentNumberUriSize );

    if ( librdf_storage_clucene_termdocs_stream_goto_next(streamData) == 0 ){
      librdf_stream* stream= librdf_new_stream(this->world,(void*)streamData,
        &librdf_storage_clucene_termdocs_stream_is_end,
        &librdf_storage_clucene_termdocs_stream_goto_next,
        &librdf_storage_clucene_termdocs_stream_get_statement,
        &librdf_storage_clucene_termdocs_stream_free
       );
       return stream;
    }else{
      librdf_storage_clucene_termdocs_stream_free(streamData);
    }
  }else if (subject && predicate && !object){
    if ( librdf_node_is_resource(subject) ){
      librdf_uri* uri = librdf_node_get_uri (subject);
      assert(uri != NULL);
      const char* uristr = (const char*)librdf_uri_as_string(uri);
      assert(uristr != NULL);

      if ( strncmp(uristr, CLuceneStorageImpl::documentNumberUri.c_str(), CLuceneStorageImpl::documentNumberUri.length() ) == 0 ){
        //one document...
        int docNum = atoi(uristr+CLuceneStorageImpl::documentNumberUri.length());
        if ( docNum >= 0 && docNum < reader->maxDoc() && !reader->isDeleted(docNum) ){
          //single document...
          librdf_storage_clucene_singledoc_stream_data* streamData;
          streamData = (librdf_storage_clucene_singledoc_stream_data*)malloc(sizeof(librdf_storage_clucene_singledoc_stream_data));
          librdf_storage_clucene_singledoc_stream_init(streamData, librdf_new_statement_from_statement(statement));

          Document* doc = reader->document(docNum);//todo: use field selector when upgrade
          wstring field = doc->get( NodeToKeyword(predicate).c_str() );
          librdf_statement_set_object(streamData->currentStatement,
            librdf_new_node_from_literal(this->world,
              (const unsigned char*)StringToChar(field).c_str(), NULL,0));
          _CLDELETE(doc);

          librdf_stream* stream= librdf_new_stream(this->world,(void*)streamData,
            &librdf_storage_clucene_singledoc_stream_is_end,
            &librdf_storage_clucene_singledoc_stream_goto_next,
            &librdf_storage_clucene_singledoc_stream_get_statement,
            &librdf_storage_clucene_singledoc_stream_free
           );
           return stream;
        }
        return librdf_new_empty_stream(this->world);
      }
    }
    assert(false);
  }
  return librdf_new_empty_stream(this->world);
}




std::wstring CLuceneStorageImpl::CharToString(const unsigned char * str){
    return CharToString( (const char*) str);
}
std::wstring CLuceneStorageImpl::CharToString(const char* str){
    size_t l = strlen(str);
    TCHAR* tmp = _CL_NEWARRAY(TCHAR,l * sizeof(TCHAR) );
    l = lucene_utf8towcs(tmp,str,l);
    wstring ret = wstring(tmp,l);
    _CLDELETE_CARRAY(tmp);
    return ret;
}
std::string CLuceneStorageImpl::StringToChar(std::wstring str){
  //todo: new version has a help function for this...
  char buf[10];
  string ret;
  ret.reserve(str.length());
  size_t l = str.length();
  size_t t = 0;
  for (size_t i=0;i<l;i++){
    t = lucene_wctoutf8(buf,str[i]);
    ret.append(buf,t);
  }
  return ret;
}
std::wstring CLuceneStorageImpl::NodeToKeyword(librdf_node* node){
    librdf_node_type type = librdf_node_get_type (node);
    if ( type == LIBRDF_NODE_TYPE_RESOURCE ){
      librdf_uri* uri = librdf_node_get_uri (node);
      assert(uri != NULL);
      return CharToString(librdf_uri_as_string(uri));
    }else if ( type == LIBRDF_NODE_TYPE_LITERAL ){
      return CharToString(librdf_node_get_literal_value (node));
    }else{
      assert(false);///
    }
}
std::string CLuceneStorageImpl::NodeToString(librdf_node* node){
    librdf_node_type type = librdf_node_get_type (node);
    if ( type == LIBRDF_NODE_TYPE_RESOURCE ){
      librdf_uri* uri = librdf_node_get_uri (node);
      assert(uri != NULL);
      return (const char*)librdf_uri_to_string(uri);
    }else if ( type == LIBRDF_NODE_TYPE_LITERAL ){
      return strdup((const char*)librdf_node_get_literal_value (node));
    }else{
      assert(false);///
    }
}
} // namespace Redland
