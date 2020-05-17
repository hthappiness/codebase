namespace base
{
	class CDict
	{
	public:
		CDict(void *privDataPtr);
		CDict(void *privDataPtr, IDictType*);/*若不传此参数，则生成默认的dictType*/

		int dictExpand(unsigned long size);
		int dictAdd(void *key, void *val);
		dictEntry *dictAddRaw(void *key, dictEntry **existing);
		dictEntry *dictAddOrFind(void *key);
		int dictReplace(void *key, void *val);
		int dictDelete(const void *key);
		dictEntry *dictUnlink(const void *key);
		void dictFreeUnlinkedEntry(dictEntry *he);
		void dictRelease();
		dictEntry * dictFind(const void *key);
		void *dictFetchValue(const void *key);
		int dictResize();
		dictIterator *dictGetIterator();
		dictIterator *dictGetSafeIterator();
		dictEntry *dictNext(dictIterator *iter);
		void dictReleaseIterator(dictIterator *iter);
		dictEntry *dictGetRandomKey();
		unsigned int dictGetSomeKeys(dictEntry **des, unsigned int count);
		void dictGetStats(char *buf, size_t bufsize, dict *d);
		uint64_t dictGenHashFunction(const void *key, int len);
		uint64_t dictGenCaseHashFunction(const unsigned char *buf, int len);
		void dictEmpty(void(callback)(void*));
		void dictEnableResize(void);
		void dictDisableResize(void);
		int dictRehash(int n);
		int dictRehashMilliseconds(int ms);
		void dictSetHashFunctionSeed(uint8_t *seed);
		uint8_t *dictGetHashFunctionSeed(void);
		unsigned long dictScan(unsigned long v, dictScanFunction *fn, dictScanBucketFunction *bucketfn, void *privdata);
		uint64_t dictGetHash(const void *key);
		dictEntry **dictFindEntryRefByPtrAndHash(const void *oldptr, uint64_t hash);

		class CDictEntry
		{

		};

		class CDictHash
		{
			CDictEntry** m_dictEntry;
		};

		/* value operation function for user */
		class IDictType
		{
			virtual uint64_t hashFunction(const void *key) = 0;
			virtual void* keyDup(void *privdata, const void *key) = 0;
	    	virtual void* valDup(void *privdata, const void *obj) = 0;
	    	virtual int keyCompare(void *privdata, const void *key1, const void *key2) = 0;
	    	virtual void keyDestructor(void *privdata, void *key) = 0;
	    	virtual void valDestructor(void *privdata, void *obj) = 0;
		};

		class CDictIterator
		{

		};

	private:
		IDictType* m_dictType;
		void*	   m_usrPrivData;
	};

	class CDictDefaultType: public IDictType
	{
		virtual uint64_t hashFunction(const void *key) = 0;
		virtual void* keyDup(void *privdata, const void *key) = 0;
	    virtual void* valDup(void *privdata, const void *obj) = 0;
	    virtual int keyCompare(void *privdata, const void *key1, const void *key2) = 0;
	    virtual void keyDestructor(void *privdata, void *key) = 0;
	    virtual void valDestructor(void *privdata, void *obj) = 0;
	};
}