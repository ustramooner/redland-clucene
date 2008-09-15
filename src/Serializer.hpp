/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * Serializer.cpp - Redland C++ Serializer class
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


#ifndef REDLANDPP_SERIALIZER_HH
#define REDLANDPP_SERIALIZER_HH


#ifdef HAVE_CONFIG_H
#include <redlandpp_config.h>
#endif

#include <World.hpp>
#include <Exception.hpp>
#include <Wrapper.hpp>
#include <Stream.hpp>
#include <Uri.hpp>


namespace Redland {

  using namespace std;

  class Serializer : public Wrapper<librdf_serializer> {
    public:
    Serializer(World* w, const string name, string mime_type, Uri* uri) throw(Exception);

    ~Serializer();

    // public methods
    const string name() const;

    const string str() const;

    librdf_serializer* serializer() const;

  protected:
      World* world_;

  private:
    string name_;

    friend ostream& operator<< (ostream& os, const Serializer& p);
    friend ostream& operator<< (ostream& os, const Serializer* p);
  };


} // namespace Redland

#endif