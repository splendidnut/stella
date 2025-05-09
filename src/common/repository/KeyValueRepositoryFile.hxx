//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2025 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef KEY_VALUE_REPOSITORY_FILE_HXX
#define KEY_VALUE_REPOSITORY_FILE_HXX

#include <sstream>

#include "KeyValueRepository.hxx"
#include "Logger.hxx"
#include "FSNode.hxx"
#include "bspf.hxx"

template<typename T>
class KeyValueRepositoryFile : public KeyValueRepository {
  public:
    KVRMap load() override;

    bool save(const KVRMap& values) override;

  private:
    explicit KeyValueRepositoryFile(const FSNode& node);
    friend T;

  protected:

    const FSNode& myNode;  // NOLINT: we want a reference here
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<typename T>
KeyValueRepositoryFile<T>::KeyValueRepositoryFile(const FSNode& node)
  : myNode{node}
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<typename T>
KVRMap KeyValueRepositoryFile<T>::load()
{
  if (!myNode.exists()) return {};

  stringstream in;

  try {
    myNode.read(in);
    return T::load(in);
  }
  catch (const runtime_error& err) {
    Logger::error(err.what());

    return {};
  }
  catch (...) {
    return {};
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<typename T>
bool KeyValueRepositoryFile<T>::save(const KVRMap& values)
{
  if (values.empty()) return true;

  stringstream out;

  try {
    T::save(out, values);
    myNode.write(out);

    return true;
  }
  catch (const runtime_error& err) {
    Logger::error(err.what());

    return false;
  }
  catch (...)
  {
    return false;
  }
}

#endif // KEY_VALUE_REPOSITORY_FILE
