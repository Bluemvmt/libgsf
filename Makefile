SUBDIRS = cjson GSF_03-08 GSF_03-09 GSF_03-10

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

GSF_03_08: cjson
GSF_03_09: cjson
GSF_03_10: cjson

clean:
	cd cjson && make clean && cd .. &&  \
	cd GSF_03-08 && make clean && cd .. && \
	cd GSF_03-09 && make clean && cd .. && \
	cd GSF_03-10 && make clean && cd ..
