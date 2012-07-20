/**
	@file
	@author Andrew W. Rose
	@author Marc Magrans De Abril
	@date 2012
*/

#ifndef _uhal_ValMem_hpp_
#define _uhal_ValMem_hpp_

#include "uhal/exception.hpp"
#include "uhal/definitions.hpp"
#include <boost/shared_ptr.hpp>

#include <vector>
#include <deque>
#include <iostream>

#include "uhal/log/log.hpp"

//! @todo Need to discuss and finalize which methods in ValWord and ValVector check validity and which don't. Currently it seems a little ad-hoc.

namespace uhal
{
	//! Exception class to handle the case of attempted access on unvalidated memory. Uses the base uhal::exception implementation of what()
	class NonValidatedMemory: public uhal::_exception< NonValidatedMemory > {  };
	//! Exception class to handle the case of attempted modification of validated memory. Uses the base uhal::exception implementation of what()
	class ValMemImutabilityViolation: public uhal::_exception< ValMemImutabilityViolation > { };

	// Forward declare PackingProtocol so it can be our friend
	class PackingProtocol;


	// Forward declaration so we can define friends
	class ValHeader;
	// Forward declaration so we can define friends
	template< typename T > class ValWord;
	// Forward declaration so we can define friends
	template< typename T > class ValVector;


	//! A helper struct wrapping an IPbus header and a valid flag
	struct _ValHeader_
	{
		public:
			//! A flag for marking whether the data is actually valid
			bool valid;
			//! The IPbus header associated with the transaction that returned this data
			uint32_t IPbusHeader;

		private:
			//! Make ValHeader a friend since it is the only class that should be able to create an instance this struct
			friend class ValHeader;
			/**
				Constructor
				Private, since this struct should only be used by the ValHeader
				@param aValid an initial validity
			*/
			_ValHeader_ ( const bool& aValid );
	};



	//! A Template helper struct wrapping an IPbus header, a register for storing a single word of data, a valid flag and a mask for modifying returned values
	template< typename T >
	struct _ValWord_
	{
		public:
			//! A register for storing data
			T value;
			//! A flag for marking whether the data is actually valid
			bool valid;
			//! A mask for modifying returned values
			uint32_t mask;
			//! The IPbus header associated with the transaction that returned this data
			uint32_t IPbusHeader;

		private:
			//! Make ValWord a friend since it is the only class that should be able to create an instance this struct
			friend class ValWord<T>;
			/**
				Constructor
				Private, since this struct should only be used by the ValWord
				@param aValue an initial value
				@param aValid an initial validity
				@param aMask a mask value
			*/
			_ValWord_ ( const T& aValue , const bool& aValid , const uint32_t aMask );
	};


	//! A Template helper struct wrapping a block of IPbus header, a register for storing a block of data and a valid flag
	template< typename T >
	struct _ValVector_
	{
		public:
			//! A block of memory for storing data
			std::vector<T> value;
			//! A flag for marking whether the data is actually valid
			bool valid;
			//! The IPbus header associated with the transaction that returned this data
			std::deque<uint32_t> IPbusHeaders;

		private:
			//! Make ValVector a friend since it is the only class that should be able to create an instance this struct
			friend class ValVector<T>;
			/**
				Constructor
				Private, since this struct should only be used by the ValVector
				@param aValue an initial value
				@param aValid an initial validity
			*/
			_ValVector_ ( const std::vector<T>& aValue , const bool& aValid );
	};



	//! A class which wraps a single word of data and marks whether or not it is valid
	class ValHeader
	{
			//! Make PackingProtocol a friend so that it can access the members to get the info associated with the raw data space
			friend class PackingProtocol;

		public:
			/**
				Default constructor
			*/
			ValHeader();
			/**
				Return whether the Validated memory is marked as valid
				@return whether the Validated memory is marked as valid
			*/
			bool valid();
			/**
				Change the validatity state of the Validated memory
				@param aValid the new validity state of the Validated memory
			*/
			void valid ( bool aValid );

		private:
			//! A shared pointer to a _ValWord_ struct, so that every copy of this ValWord points to the same underlying memory
			boost::shared_ptr< _ValHeader_ > mMembers;
	};


	//! A class which wraps a single word of data and marks whether or not it is valid
	template< typename T >
	class ValWord
	{

			//! Make PackingProtocol a friend so that it can access the members to get the info associated with the raw data space
			friend class PackingProtocol;

		public:
			/**
				Constructor
				@param aValue a value to which the validated memory will be initialized
				@param aMask a mask for modifying returned values
			*/
			ValWord ( const T& aValue , const uint32_t& aMask = defs::NOMASK );
			/**
				Copy constructor
				@param aVal a ValWord to copy
			*/
			ValWord ( const ValWord<T>& aVal );
			/**
				Default constructor
			*/
			ValWord();
			/**
				Return whether the Validated memory is marked as valid
				@return whether the Validated memory is marked as valid
			*/
			bool valid();
			/**
				Change the validatity state of the Validated memory
				@param aValid the new validity state of the Validated memory
			*/
			void valid ( bool aValid );
			/**
				Assignment operator - no check on whether the data has previously been marked as valid
				@param aValue Change the value stored in the Validated memory
				@return aReference to this ValWord for chained expressions
				@todo should there be a validity check here
			*/
			ValWord& operator = ( const T& aValue );
			/**
				Return the value of the validated memory without checking validity
				@return the value of the validated memory
				@todo should there be a validity check here
			*/
			operator T();
			/**
				Return the value of the validated memory with check on validity
				@return the value of the validated memory
			*/
			T value() const;
			/**
				If the memory has not previously been marked as valid, set the value
				@param aValue the value of the validated memory
			*/
			void value ( const T& aValue );
			/**
				Return the mask used by this validated memory
				@return the mask used by this validated memory
			*/
			const uint32_t& mask() const;
			/**
				Set the mask used by this validated memory
				@param aMask the mask to be used by this validated memory
			*/
			void mask ( const uint32_t& aMask );

		private:
			//! A shared pointer to a _ValWord_ struct, so that every copy of this ValWord points to the same underlying memory
			boost::shared_ptr< _ValWord_<T> > mMembers;

	};

	//! A class which wraps a block of data and marks whether or not it is valid
	template< typename T >
	class ValVector
	{
			//! Make PackingProtocol a friend so that it can access the members to get the info associated with the raw data space
			friend class PackingProtocol;

		public:
			//! typedef iterator to be that of the underlying storage type
			typedef typename std::vector< T >::iterator iterator;
			//! typedef iterator to be that of the underlying storage type
			typedef typename std::vector< T >::const_iterator const_iterator;
			//! typedef iterator to be that of the underlying storage type
			typedef typename std::vector< T >::reverse_iterator reverse_iterator;
			//! typedef iterator to be that of the underlying storage type
			typedef typename std::vector< T >::const_reverse_iterator const_reverse_iterator;

		public:
			/**
				Constructor
				@param aValues a vector of values to which the validated memory will be initialized
			*/
			ValVector ( const std::vector<T>& aValues );
			/**
				Copy Constructor
				@param aValues a ValVector to copy
			*/
			ValVector ( const ValVector& aValues );
			/**
				Constructor
				@param aSize the initial size of the validated memory
			*/
			ValVector ( uint32_t aSize );
			/**
				Default constructor
			*/
			ValVector();
			/**
				Return whether the Validated memory is marked as valid
				@return whether the Validated memory is marked as valid
			*/
			bool valid();
			/**
				Change the validatity state of the Validated memory
				@param aValid the new validity state of the Validated memory
			*/
			void valid ( bool aValid );
			/**
				Assignment operator - including check on whether the data has previously been marked as valid
				@param aBegin Iterator to the beginning of a block of data to be copied to the Validated Memory
				@param aEnd Iterator to the end (one past last entry) of a block of data to be copied to the Validated Memory
			*/
			template <class InputIterator> void assign ( InputIterator aBegin , InputIterator aEnd );
			/**
				If the memory has not previously been marked as valid, add an entry to the end of it
				@param aValue aValue to append to the block
			*/
			void push_back ( const T& aValue );
			/**
				If the memory has previously been marked as valid, give random access into memory
				@param aIndex index of the data to be retrieved
				@return a const reference to the data at the specified index
			*/
			const T& operator[] ( std::size_t aIndex ) const;
			/**
				If the memory has previously been marked as valid, give random access into memory
				@param aIndex index of the data to be retrieved
				@return a const reference to the data at the specified index
			*/
			const T& at ( std::size_t aIndex ) const;
			/**
				Return the size of the underlying memory
				@return the size of the underlying memory
			*/
			std::size_t size() const;
			/**
				Clear the underlying memory and set Validity to false
			*/
			void clear();
			/**
				If the memory has previously been marked as valid, return a const iterator to the beginning of the underlying memory
				@return a const iterator to the beginning of the underlying memory
			*/
			const_iterator begin() const;
			/**
				If the memory has previously been marked as valid, return a const iterator to the end (one past last entry) of the underlying memory
				@return a const iterator to the end (one past last entry) of the underlying memory
			*/
			const_iterator end() const;
			/**
				If the memory has previously been marked as valid, return a const reverse iterator to the reverse beginning (the last entry) of the underlying memory
				@return a const reverse iterator to the reverse beginning (the last entry) of the underlying memory
			*/
			const_reverse_iterator rbegin() const;
			/**
				If the memory has previously been marked as valid, return a const reverse iterator to the reverse end (one before the zero'th entry) of the underlying memory
				@return a const reverse iterator to the reverse end (one before the zero'th entry) of the underlying memory
			*/
			const_reverse_iterator rend() const;

			/*
						/ **
							If the memory has not previously been marked as valid, return a iterator to the beginning of the underlying memory
							@return a iterator to the beginning of the underlying memory
						* /
						iterator begin();
						/ **
							If the memory has not previously been marked as valid, return a iterator to the end (one past last entry) of the underlying memory
							@return a iterator to the end (one past last entry) of the underlying memory
						* /
						iterator end();
						/ **
							If the memory has not previously been marked as valid, return a reverse iterator to the reverse beginning (the last entry) of the underlying memory
							@return a reverse iterator to the reverse beginning (the last entry) of the underlying memory
						* /
						reverse_iterator rbegin();
						/ **
							If the memory has not previously been marked as valid, return a reverse iterator to the reverse end (one before the zero'th entry) of the underlying memory
							@return a reverse iterator to the reverse end (one before the zero'th entry) of the underlying memory
						* /
						reverse_iterator rend();
			*/


		private:
			//! A shared pointer to a _ValVector_ struct, so that every copy of this ValVector points to the same underlying memory
			boost::shared_ptr< _ValVector_<T> > mMembers;

	};

}

#include "uhal/TemplateDefinitions/ValMem.hxx"

#endif

