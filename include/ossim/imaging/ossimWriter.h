//---
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Generic image writer class.
// 
//---
// $Id$
#ifndef ossimWriter_HEADER
#define ossimWriter_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <iosfwd>
#include <vector>

/**
 * @brief ossimWriter - Generic image writer.
 *
 * Please do NOT add any external library dependencies. drb - 26 May 2016
 */
class OSSIM_DLL ossimWriter : public ossimImageFileWriter
{
public:
   
   /** default constructor */
   ossimWriter();

   /** virtual destructor */
   virtual ~ossimWriter();

   /** @return "ossim_writer" */
   virtual ossimString getShortName() const;

   /** @return "ossim writer" */
   virtual ossimString getLongName() const;

   /** @return "ossimWriter" */
   virtual ossimString getClassName() const;

   /**
    * Returns a 3-letter extension from the image type descriptor 
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;
   
   /**
    * @brief getImageTypeList method.
    *
    * Satisfies ossimImageFileWriter::getImageTypeList pure virtual.
    * 
    * Appends the writers image types to the "imageTypeList".
    * 
    * Current write type:
    * ossim_ttbs - tiled tiff band separate, big tiff format.
    *
    * @param imageTypeList list to append to.
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList) const;

   /**
    * @brief isOpen
    * @return true if open; else, false.
    */
   virtual bool isOpen()const; 

   /**
    * @brief open
    * @return true on success, false on error.
    */
   virtual bool open();

   /** @brief close Flushes and deletes stream if we own it. */
   virtual void close();
   
   bool hasImageType(const ossimString& imageType) const;

   /**
    * @brief Method to write the image to a stream.
    *
    * @return true on success, false on error.
    */
   virtual bool writeStream();

   /**
    * @brief Sets the output stream to write to.
    *
    * This sets the stream and sets m_ownsStreamFlag to false. So destructor
    * will not delete the stream if this method is used.
    * 
    * @param stream The stream to write to.
    * @return true if object can write to stream, false if not.
    */
   virtual bool setOutputStream(std::ostream& str);

protected:
   
   /**
    * @brief Write out the file.
    * @return true on success, false on error.
    */
   virtual bool writeFile();

private:

   /**
    * @brief Writes a tiled tiff band separate to stream.
    * @return true on success, false on error.
    */
   bool writeStreamTtbs();

   /**
    * @brief Writes tiff header to stream.
    * @return true on success, false on error.
    */
   bool writeTiffHdr();

   /**
    * @brief Writes tags to image file directory(IFD).
    * @param tile_offsets
    * @param tile_byte_counts
    * @param minBands
    * @param maxBands
    * @return true on success, false on error.
    */
   bool writeTiffTags( const std::vector<ossim_uint64>& tile_offsets,
                       const std::vector<ossim_uint64>& tile_byte_counts,
                       const std::vector<ossim_float64>& minBands,
                       const std::vector<ossim_float64>& maxBands );

   /**
    * @brief Writes tags TIFFTAG_MINSAMPLEVALUE(280) and
    * TIFFTAG_MAXSAMPLEVALUE(281).  Only written if scalar type is an unsigned
    * byte or short.
    * @param minBands Array of min values from image write.
    * @param maxBands Array of max values from image write.
    * @return true on success, false on error.
    */
   bool writeMinMaxTiffTags( const std::vector<ossim_float64>& minBands,
                             const std::vector<ossim_float64>& maxBands,
                             std::streamoff& arrayWritePos  );


   /**
    * @brief Writes tags TIFFTAG_SMINSAMPLEVALUE(340) and
    * TIFFTAG_SMAXSAMPLEVALUE(341).  Only written if scalar type is not an
    * unsigned byte or short.
    * @param minBands Array of min values from image write.
    * @param maxBands Array of max values from image write.
    * @return true on success, false on error.
    */
   bool writeSMinSMaxTiffTags( const std::vector<ossim_float64>& minBands,
                             const std::vector<ossim_float64>& maxBands,
                             std::streamoff& arrayWritePos  );

   /**
    * @brief Writes tiff tag to image file directory(IFD).
    * @param tag
    * @param type
    * @param count
    * @param value(s) or offset to array.
    * @param arrayWritePos Position to write array to.  This will be updated
    * if array is written with new offset.
    */   
   template <class T> void writeTiffTag(ossim_uint16 tag, ossim_uint16 type,
                                        ossim_uint64 count,
                                        const T* value,
                                        std::streamoff& arrayWritePos );

   /**
    * @brief Writes image data to stream.
    *
    * Data is in a band separate tile layout(PLANARCONFIG_SEPARATE), i.e. all
    * the red tiles, all the green tiles, all the blue tiles.
    * 
    * @param tile_offsets Initialized by this with offset for each tile.
    * @param tile_byte_counts Initialized by this with the byte count of each
    * tile.
    * @param minBands Initialized by this with the min values for each band.
    * @param maxBands Initialized by this with the max values for each band.
    * @return true on success, false on error.
    */
   bool writeTiffTilesBandSeparate( std::vector<ossim_uint64>& tile_offsets,
                                    std::vector<ossim_uint64>& tile_byte_counts,
                                    std::vector<ossim_float64>& minBands,
                                    std::vector<ossim_float64>& maxBands );

   /**
    * @brief Gets the tiff sample format based on scalar type.
    * E.g SAMPLEFORMAT_UINT, SAMPLEFORMAT_INT or SAMPLEFORMAT_IEEEFP.
    * @return TIFF sample format or 0 if not mapped to a scalar type.
    */
   ossim_uint16 getTiffSampleFormat() const;
  
   std::ostream* m_str;
   bool          m_ownsStreamFlag; 
   
}; // End: class ossimWriter

#endif /* End of "#ifndef ossimWriter_HEADER" */
