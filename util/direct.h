// direct.h v2.2
// August 2001
// krys@via.ecp.fr

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.


// history
// v 1.0 TACCRes
// v 2.0 TACCEditMem
// v 2.1 header ACC modified
// v 2.2 TACCIndex

// TACCRes: ACC file reader (read from memory)
// TACCEditMem: can add, delete, extract from memory
// TACCIndex: ACC file reader (read from disk)

#ifndef __DIRECT
#define __DIRECT "v2.2"

#define ACC_OK 0

#define ACC_NOTENOUGHMEMORY -101
#define ACC_INVALIDPARAMETER -102
#define ACC_FILENOTFOUND -111
#define ACC_FILECORRUPT -112
#define ACC_ENTRYNOTFOUND -113
#define ACC_CANNOTCREATEFILE -114
#define ACC_DISKFULL -115
#define ACC_FSEEKFAILED -116

#define ACC_NOTINITIALIZED -128
#define ACC_NOTIMPLEMENTED -129

#define TEntrySize 19	// length of entry names
						// this value is arbitrary
                        // Note: the filename are stored without the path
						
typedef struct { char Name[TEntrySize];
                 unsigned long Length;
					  char* pData;
               } TEntry, *PEntry;

typedef TEntry TEntryTable, *PEntryTable;

class TACCRes { public:
                 PEntryTable pTable;      // points to the entry table
                 unsigned NbEntry;
                 char* CurrentFile;       // current ACC file

                 inline TACCRes()  
				{ pTable= NULL;
                  CurrentFile= NULL;
                  NbEntry= 0; }
                 inline ~TACCRes() { Done(); }
                 void Done ();

                 long EntryId (char* f);            // Entry's id
                 char* EntryName(unsigned Id);      // Entry's name
                 char* EntryPtr (unsigned Id);      // Entry's data pointer in memory
                 unsigned long EntryLength (unsigned Id);

                 signed char LoadACC (char* f);   // Load an ACC file
               };

class TACCIndex { public:
				 PEntryTable pTable;		// points to the entry table
                 unsigned NbEntry;
                 unsigned long HeaderSize;	// ACC files consists of an header and
                							// the concatenation of all the data
                 							// The header size is given by this method
                 char* CurrentFile;			// current ACC file
                 FILE* file; 				// data file, opened by LoadACC()
                 							// closed by Done(), positioned by
                                          	// SetPosition()

                 inline TACCIndex()
                  { pTable= NULL;
                    CurrentFile= NULL;
                    file= NULL;
                    NbEntry= 0; }
                 inline ~TACCIndex() { Done(); }
                 void Done ();

                 long EntryId (char* f);            // Entry's id
                 char* EntryName (unsigned Id);     // Entry's name
                 unsigned long EntryOffset (unsigned Id);	// Data offset in file
                 unsigned long EntryLength (unsigned Id);
                 signed char SetPosition (unsigned Id);	// similar to fseek

                 signed char LoadACC (char* f);   // Load an ACC file
               };


class TACCEditMem: public TACCRes
       { public:
          inline TACCEditMem()  { pTable= NULL;
                                  CurTabSize= 0; }
          inline ~TACCEditMem() { Done(); }

          signed char AddEntry (char* f, unsigned Pos);
          signed char DelEntry (unsigned Id);
          void Done();
          signed char ExtractEntry (unsigned Id, char* f);
                                      // if f== NULL then f= pTable[Id].Name
          signed char LoadACC (char* f);
          signed char SaveACC (char* f);
                                      // if f== NULL then f= CurrentFile
         protected:
          unsigned long CurTabSize;
       };

#endif
