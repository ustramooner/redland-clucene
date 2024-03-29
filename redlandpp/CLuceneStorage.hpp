/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * Storage.cpp - Redland C++ Storage class interface
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

#ifndef REDLANDPP_CLUCENE_STORAGE_HPP
#define REDLANDPP_CLUCENE_STORAGE_HPP

#ifdef HAVE_CONFIG_H
#include <redlandpp_config.h>
#endif

#include <redland.h>

#include "redlandpp/World.hpp"
#include "redlandpp/Exception.hpp"
#include "redlandpp/Wrapper.hpp"
#include "redlandpp/Stream.hpp"
#include "redlandpp/Uri.hpp"
#include "redlandpp/Storage.hpp"

namespace Redland {
  class CLuceneStorage : public Storage {
  public:
    CLuceneStorage(World* w, const std::string& name="", const std::string& opts="") throw(Exception);
    CLuceneStorage(World& w, const std::string& name="", const std::string& opts="") throw(Exception);
    ~CLuceneStorage();

    static void initialise(World& world);
    static void shutdown();
  };

} // namespace Redland

#endif
