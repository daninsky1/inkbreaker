//
// Created by daniel on 10/08/2025.
//

#include "blend2d_utils.h"

namespace gfx {
std::string getBLError(BLResult result)
{
    switch (result) {
    case BL_SUCCESS:
        return "BL_SUCCESS";
    case BL_ERROR_OUT_OF_MEMORY:
        return "Out of memory                 [ENOMEM].";
    case BL_ERROR_INVALID_VALUE:
        return "Invalid value/argument        [EINVAL].";
    case BL_ERROR_INVALID_STATE:
        return "Invalid state                 [EFAULT].";
    case BL_ERROR_INVALID_HANDLE:
        return "Invalid handle or file.       [EBADF].";
    case BL_ERROR_INVALID_CONVERSION:
        return "Invalid conversion.";
    case BL_ERROR_OVERFLOW:
        return "Overflow or value too large   [EOVERFLOW].";
    case BL_ERROR_NOT_INITIALIZED:
        return "Object not initialized.";
    case BL_ERROR_NOT_IMPLEMENTED:
        return "Not implemented               [ENOSYS].";
    case BL_ERROR_NOT_PERMITTED:
        return "Operation not permitted       [EPERM].";
    case BL_ERROR_IO:
        return "IO error                      [EIO].";
    case BL_ERROR_BUSY:
        return "Device or resource busy       [EBUSY].";
    case BL_ERROR_INTERRUPTED:
        return "Operation interrupted         [EINTR].";
    case BL_ERROR_TRY_AGAIN:
        return "Try again                     [EAGAIN].";
    case BL_ERROR_TIMED_OUT:
        return "Timed out                     [ETIMEDOUT].";
    case BL_ERROR_BROKEN_PIPE:
        return "Broken pipe                   [EPIPE].";
    case BL_ERROR_INVALID_SEEK:
        return "File is not seekable          [ESPIPE].";
    case BL_ERROR_SYMLINK_LOOP:
        return "Too many levels of symlinks   [ELOOP].";
    case BL_ERROR_FILE_TOO_LARGE:
        return "File is too large             [EFBIG].";
    case BL_ERROR_ALREADY_EXISTS:
        return "File/directory already exists [EEXIST].";
    case BL_ERROR_ACCESS_DENIED:
        return "Access denied                 [EACCES].";
    case BL_ERROR_MEDIA_CHANGED:
        return "Media changed                 [Windows::ERROR_MEDIA_CHANGED].";
    case BL_ERROR_READ_ONLY_FS:
        return "The file/FS is read-only      [EROFS].";
    case BL_ERROR_NO_DEVICE:
        return "Device doesn't exist          [ENXIO].";
    case BL_ERROR_NO_ENTRY:
        return "Not found, no entry (fs)      [ENOENT].";
    case BL_ERROR_NO_MEDIA:
        return "No media in drive/device      [ENOMEDIUM].";
    case BL_ERROR_NO_MORE_DATA:
        return "No more data / end of file    [ENODATA].";
    case BL_ERROR_NO_MORE_FILES:
        return "No more files                 [ENMFILE].";
    case BL_ERROR_NO_SPACE_LEFT:
        return "No space left on device       [ENOSPC].";
    case BL_ERROR_NOT_EMPTY:
        return "Directory is not empty        [ENOTEMPTY].";
    case BL_ERROR_NOT_FILE:
        return "Not a file                    [EISDIR].";
    case BL_ERROR_NOT_DIRECTORY:
        return "Not a directory               [ENOTDIR].";
    case BL_ERROR_NOT_SAME_DEVICE:
        return "Not same device               [EXDEV].";
    case BL_ERROR_NOT_BLOCK_DEVICE:
        return "Not a block device            [ENOTBLK].";
    case BL_ERROR_INVALID_FILE_NAME:
        return "File/path name is invalid     [n/a].";
    case BL_ERROR_FILE_NAME_TOO_LONG:
        return "File/path name is too long    [ENAMETOOLONG].";
    case BL_ERROR_TOO_MANY_OPEN_FILES:
        return "Too many open files           [EMFILE].";
    case BL_ERROR_TOO_MANY_OPEN_FILES_BY_OS:
        return "Too many open files by OS     [ENFILE].";
    case BL_ERROR_TOO_MANY_LINKS:
        return "Too many symbolic links on FS [EMLINK].";
    case BL_ERROR_TOO_MANY_THREADS:
        return "Too many threads              [EAGAIN].";
    case BL_ERROR_THREAD_POOL_EXHAUSTED:
        return "Thread pool is exhausted and couldn't acquire the requested thread count.";
    case BL_ERROR_FILE_EMPTY:
        return "File is empty (not specific to any OS error).";
    case BL_ERROR_OPEN_FAILED:
        return "File open failed              [Windows::ERROR_OPEN_FAILED].";
    case BL_ERROR_NOT_ROOT_DEVICE:
        return "Not a root device/directory   [Windows::ERROR_DIR_NOT_ROOT].";
    case BL_ERROR_UNKNOWN_SYSTEM_ERROR:
        return "Unknown system error that failed to translate to Blend2D result code.";
    case BL_ERROR_INVALID_ALIGNMENT:
        return "Invalid data alignment.";
    case BL_ERROR_INVALID_SIGNATURE:
        return "Invalid data signature or header.";
    case BL_ERROR_INVALID_DATA:
        return "Invalid or corrupted data.";
    case BL_ERROR_INVALID_STRING:
        return "Invalid string (invalid data of either UTF8, UTF16, or UTF32).";
    case BL_ERROR_INVALID_KEY:
        return "Invalid key or property.";
    case BL_ERROR_DATA_TRUNCATED:
        return "Truncated data (more data required than memory/stream provides).";
    case BL_ERROR_DATA_TOO_LARGE:
        return "Input data too large to be processed.";
    case BL_ERROR_DECOMPRESSION_FAILED:
        return "Decompression failed due to invalid data (RLE, Huffman, etc).";
    case BL_ERROR_INVALID_GEOMETRY:
        return "Invalid geometry (invalid path data or shape).";
    case BL_ERROR_NO_MATCHING_VERTEX:
        return "Returned when there is no matching vertex in path data.";
    case BL_ERROR_INVALID_CREATE_FLAGS:
        return "Invalid create flags (BLContext).";
    case BL_ERROR_NO_MATCHING_COOKIE:
        return "No matching cookie (BLContext).";
    case BL_ERROR_NO_STATES_TO_RESTORE:
        return "No states to restore (BLContext).";
    case BL_ERROR_TOO_MANY_SAVED_STATES:
        return "Cannot save state as the number of saved states reached the limit (BLContext).";
    case BL_ERROR_IMAGE_TOO_LARGE:
        return "The size of the image is too large.";
    case BL_ERROR_IMAGE_NO_MATCHING_CODEC:
        return "Image codec for a required format doesn't exist.";
    case BL_ERROR_IMAGE_UNKNOWN_FILE_FORMAT:
        return "Unknown or invalid file format that cannot be read.";
    case BL_ERROR_IMAGE_DECODER_NOT_PROVIDED:
        return "Image codec doesn't support reading the file format.";
    case BL_ERROR_IMAGE_ENCODER_NOT_PROVIDED:
        return "Image codec doesn't support writing the file format.";
    case BL_ERROR_PNG_MULTIPLE_IHDR:
        return "Multiple IHDR chunks are not allowed (PNG).";
    case BL_ERROR_PNG_INVALID_IDAT:
        return "Invalid IDAT chunk (PNG).";
    case BL_ERROR_PNG_INVALID_IEND:
        return "Invalid IEND chunk (PNG).";
    case BL_ERROR_PNG_INVALID_PLTE:
        return "Invalid PLTE chunk (PNG).";
    case BL_ERROR_PNG_INVALID_TRNS:
        return "Invalid tRNS chunk (PNG).";
    case BL_ERROR_PNG_INVALID_FILTER:
        return "Invalid filter type (PNG).";
    case BL_ERROR_JPEG_UNSUPPORTED_FEATURE:
        return "Unsupported feature (JPEG).";
    case BL_ERROR_JPEG_INVALID_SOS:
        return "Invalid SOS marker or header (JPEG).";
    case BL_ERROR_JPEG_INVALID_SOF:
        return "Invalid SOF marker (JPEG).";
    case BL_ERROR_JPEG_MULTIPLE_SOF:
        return "Multiple SOF markers (JPEG).";
    case BL_ERROR_JPEG_UNSUPPORTED_SOF:
        return "Unsupported SOF marker (JPEG).";
    case BL_ERROR_FONT_NOT_INITIALIZED:
        return "Font doesn't have any data as it's not initialized.";
    case BL_ERROR_FONT_NO_MATCH:
        return "Font or font face was not matched (BLFontManager).";
    case BL_ERROR_FONT_NO_CHARACTER_MAPPING:
        return "Font has no character to glyph mapping data.";
    case BL_ERROR_FONT_MISSING_IMPORTANT_TABLE:
        return "Font has missing an important table.";
    case BL_ERROR_FONT_FEATURE_NOT_AVAILABLE:
        return "Font feature is not available.";
    case BL_ERROR_FONT_CFF_INVALID_DATA:
        return "Font has an invalid CFF data.";
    case BL_ERROR_FONT_PROGRAM_TERMINATED:
        return "Font program terminated because the execution reached the limit.";
    case BL_ERROR_GLYPH_SUBSTITUTION_TOO_LARGE:
        return "Glyph substitution requires too much space and was terminated.";
    case BL_ERROR_INVALID_GLYPH:
        return "Invalid glyph identifier.";
    default:
        return "Unknown error";
    }
}

}