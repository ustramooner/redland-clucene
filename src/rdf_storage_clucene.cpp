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

#define LIBRDF_INTERNAL //we need access to internals...

#ifdef HAVE_CONFIG_H
#include <rdf_config.h>
#endif

#ifdef WIN32
#include <win32_rdf_config.h>
#endif

#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h> /* for abort() as used in errors */
#endif
#include <iostream>

#include <redland.h>
#include <rdf_storage.h>
#include <rdf_storage_module.h>
#include "redlandpp/Exception.hpp"
#include "redlandpp/rdf_storage_clucene.hpp"
#include "redlandpp/World.hpp"
#include "redlandpp/Wrapper.hpp"

//constructor for singleton which registers the CLucene storage
Redland::CLuceneStorageImpl::Singleton::Singleton(World& world, std::string storage_name, std::string storage_description){
  //tell rdf about our storage type...
  librdf_storage_register_factory(world.cobj(), storage_name.c_str(), storage_description.c_str(),
                              &librdf_storage_clucene_register_factory);
}
Redland::CLuceneStorageImpl::Singleton::~Singleton(){
}
//the singleton construtor
Redland::CLuceneStorageImpl::Singleton* Redland::CLuceneStorageImpl::singleton = NULL;
void Redland::CLuceneStorage::initialise(World& world){
  if ( Redland::CLuceneStorageImpl::singleton == NULL ){
      Redland::CLuceneStorageImpl::singleton = new Redland::CLuceneStorageImpl::Singleton(world, "clucene", "clucene backend");
  }
}
void Redland::CLuceneStorage::shutdown(){
  if ( Redland::CLuceneStorageImpl::singleton ){
    delete Redland::CLuceneStorageImpl::singleton;
    Redland::CLuceneStorageImpl::singleton = NULL;
  }
}

/** Local entry point for dynamically loaded storage module */
static void
librdf_storage_clucene_register_factory(librdf_storage_factory *factory)
{
  LIBRDF_ASSERT_CONDITION(!strcmp(factory->name, "clucene"));

  factory->version                         = LIBRDF_STORAGE_INTERFACE_VERSION;
  factory->init                            = librdf_storage_clucene_init;
  factory->clone                           = librdf_storage_clucene_clone;
  factory->terminate                       = librdf_storage_clucene_terminate;
  factory->open                            = librdf_storage_clucene_open;
  factory->close                           = librdf_storage_clucene_close;
  factory->size                            = librdf_storage_clucene_size;

  factory->contains_statement              = librdf_storage_clucene_contains_statement;
  factory->serialise                       = librdf_storage_clucene_serialise;

  factory->find_statements                 = librdf_storage_clucene_find_statements;
  factory->find_statements_in_context      = librdf_storage_clucene_find_statements_in_context;
  factory->find_statements_with_options    = librdf_storage_clucene_find_statements_with_options;
  //factory->find_sources                    = librdf_storage_clucene_find_sources;
  //factory->find_arcs                       = librdf_storage_clucene_find_arcs;
  //factory->find_arcs_in                    = librdf_storage_clucene_find_arcs_in;
  //factory->find_arcs_out                   = librdf_storage_clucene_find_arcs_out;
  //factory->has_arc_in                      = librdf_storage_clucene_has_arcs_out;
  //factory->has_arc_out                     = librdf_storage_clucene_has_arcs_out;
  //factory->find_targets                    = librdf_storage_clucene_find_targets;

  //factory->sync                            = librdf_storage_clucene_sync;
  //factory->get_contexts                    = librdf_storage_clucene_get_contexts;
  //factory->get_feature                     = librdf_storage_clucene_get_feature;

  //factory->transaction_start               = librdf_storage_clucene_transaction_start;
  //factory->transaction_commit              = librdf_storage_clucene_transaction_commit;
}


typedef struct
{
  Redland::CLuceneStorageImpl* instance;
} librdf_storage_clucene_instance;


//initialise the storage...
static int librdf_storage_clucene_init(librdf_storage* storage, const char *name, librdf_hash* options){
  //create the instance data...
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)LIBRDF_CALLOC(
    librdf_storage_clucene_instance, 1, sizeof(librdf_storage_clucene_instance));
  //and assign it to the storage
  storage->instance = (librdf_storage_instance)context;

  //and assign the C++ object.
  Redland::WorldRef w(storage->world);
  context->instance = new Redland::CLuceneStorageImpl(storage, storage->world);

  string _name = "";
  if ( name != 0 ) _name = name;
  context->instance->Init(_name,options);
  return 0;
}
static void librdf_storage_clucene_terminate(librdf_storage* storage){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;

  delete context->instance;

  LIBRDF_FREE(librdf_storage_clucene_instance, context);
}
static int librdf_storage_clucene_clone(librdf_storage* new_storage, librdf_storage* old_storage){
  assert(false);
  librdf_storage_clucene_instance* old_context=(librdf_storage_clucene_instance*)old_storage->instance;

  //create the instance data...
  librdf_storage_clucene_instance* newcontext=(librdf_storage_clucene_instance*)LIBRDF_CALLOC(
    librdf_storage_clucene_instance, 1, sizeof(librdf_storage_clucene_instance));
  //and assign it to the storage
  new_storage->instance = (librdf_storage_instance)newcontext;
  //and clone the c++ object.
  newcontext->instance = new Redland::CLuceneStorageImpl(*old_context->instance);
}
static int librdf_storage_clucene_open(librdf_storage* storage, librdf_model* model){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  return context->instance->Open(model);
}
static int librdf_storage_clucene_close(librdf_storage* storage){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  return context->instance->Close();
}
static int librdf_storage_clucene_size(librdf_storage* storage){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  return context->instance->Size();
}
static int librdf_storage_clucene_contains_statement(librdf_storage* storage, librdf_statement* statement){
  assert(false);
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  return 0; //TODO:
}
static librdf_stream* librdf_storage_clucene_serialise(librdf_storage* storage){
  assert(false);
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  return NULL; //TODO:
}
static librdf_stream* librdf_storage_clucene_find_statements_with_options(librdf_storage* storage, librdf_statement* statement,
                                                  librdf_node* context_node,
                                                  librdf_hash* options){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  return context->instance->FindStatementsWithOptions(statement, context_node, options);
}
static librdf_iterator* librdf_storage_clucene_find_sources(librdf_storage* storage, librdf_node* arc, librdf_node *target){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  assert(false);
  return NULL; //TODO:
}
static librdf_iterator* librdf_storage_clucene_find_arcs(librdf_storage* storage, librdf_node* source, librdf_node *target){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  assert(false);
  return NULL; //TODO:
}
static librdf_iterator* librdf_storage_clucene_find_targets(librdf_storage* storage, librdf_node* source, librdf_node *arc){
  librdf_storage_clucene_instance* context=(librdf_storage_clucene_instance*)storage->instance;
  assert(false);
  return NULL; //TODO:
}




//==============ALIASES===============

static librdf_stream*
librdf_storage_clucene_find_statements(librdf_storage* storage,
                                     librdf_statement* statement)
{
  return librdf_storage_clucene_find_statements_in_context(storage,statement,NULL);
}
static librdf_stream*
librdf_storage_clucene_find_statements_in_context(librdf_storage* storage, librdf_statement* statement,
                         librdf_node* context_node)
{
  return librdf_storage_clucene_find_statements_with_options(storage, statement, context_node, NULL);
}
