#pragma once

using namespace std;

/** \brief Interface for all encoders. */
class ICoder
{
	public:
		virtual ~ICoder() {}
		/** \brief Encodes file with path to file with pathTo by method. */
		virtual void encode(const string& path, const string& pathTo) = 0;
		/** \brief Decodes file with path to file with pathTo by method. */
		virtual void decode(const string& path, const string& pathTo) = 0;

	protected:
		ICoder() {}
};
