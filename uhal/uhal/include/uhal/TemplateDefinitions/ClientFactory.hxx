
namespace uhal
{

	template <class T>
	void ClientFactory::add ( const std::string& aProtocol , const std::string& aDescription )
	{
		try
		{
			std::hash_map<std::string , boost::shared_ptr<CreatorInterface> >::const_iterator lIt = mCreators.find ( aProtocol ) ;

			if ( lIt != mCreators.end() )
			{
				//log ( Error() , "Throwing at " , ThisLocation() );
				//ProtocolAlreadyExist().throwFrom( ThisLocation() );
				log ( Warning() , "Protocol \"" , aProtocol , "\" already exists in map of creators. Continuing for now, but be warned." );
				return;
			}

			mCreators[aProtocol] =  boost::shared_ptr<CreatorInterface> ( new Creator<T>() );

			if ( aDescription.size() == 0 )
			{
				mProductDescriptions.insert ( std::make_pair ( aProtocol , T::description() ) );
			}
			else
			{
				mProductDescriptions.insert ( std::make_pair ( aProtocol , aDescription ) );
			}
		}
		catch ( uhal::exception& aExc )
		{
			aExc.rethrowFrom ( ThisLocation() );
		}
		catch ( const std::exception& aExc )
		{
			log ( Error() , "Exception \"" , aExc.what() , "\" caught at " , ThisLocation() );
			StdException ( aExc ).throwFrom ( ThisLocation() );
		}
	}


	template <class T>
	boost::shared_ptr<ClientInterface> ClientFactory::Creator<T>::create ( const std::string& aId , const URI& aUri )
	{
		try
		{
			return boost::shared_ptr<ClientInterface> ( new T ( aId , aUri ) );
		}
		catch ( uhal::exception& aExc )
		{
			aExc.rethrowFrom ( ThisLocation() );
		}
		catch ( const std::exception& aExc )
		{
			log ( Error() , "Exception \"" , aExc.what() , "\" caught at " , ThisLocation() );
			StdException ( aExc ).throwFrom ( ThisLocation() );
		}
	}


}
