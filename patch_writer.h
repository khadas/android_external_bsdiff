// Copyright 2017 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BSDIFF_PATCH_WRITER_H_
#define _BSDIFF_PATCH_WRITER_H_

#include <memory>
#include <string>
#include <vector>

#include "bsdiff/compressor_interface.h"
#include "bsdiff/patch_writer_interface.h"

namespace bsdiff {

// A PatchWriterInterface class using the upstream's BSDIFF40 format: three
// BZ2-compressors and a 32-byte header.
class BsdiffPatchWriter : public PatchWriterInterface {
 public:
  // Create the patch writer using |type| as the compression algorithm and the
  // file |patch_filename| to write the patch data.
  BsdiffPatchWriter(const std::string& patch_filename, BsdiffFormat format);

  // PatchWriterInterface overrides.
  bool Init(size_t new_size) override;
  bool WriteDiffStream(const uint8_t* data, size_t size) override;
  bool WriteExtraStream(const uint8_t* data, size_t size) override;
  bool AddControlEntry(const ControlEntry& entry) override;
  bool Close() override;

 private:
  // Write the BSDIFF patch header to the |fp_| given the size of the compressed
  // control block and the compressed diff block.
  bool WriteHeader(uint64_t ctrl_size, uint64_t diff_size);

  // Bytes of the new files already written. Needed to store the new length in
  // the header of the file.
  uint64_t written_output_{0};

  // The current file we are writing to.
  FILE* fp_{nullptr};
  std::string patch_filename_;

  // The format of bsdiff we're using.
  BsdiffFormat format_;

  // The three internal compressed streams.
  std::unique_ptr<CompressorInterface> ctrl_stream_{nullptr};
  std::unique_ptr<CompressorInterface> diff_stream_{nullptr};
  std::unique_ptr<CompressorInterface> extra_stream_{nullptr};
};

}  // namespace bsdiff

#endif  // _BSDIFF_PATCH_WRITER_H_
