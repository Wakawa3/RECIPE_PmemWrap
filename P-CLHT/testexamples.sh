TEST_ROOT=${HOME}/workloads/RECIPE/P-CLHT

PMEMWRAP_ROOT=${HOME}/PmemWrap
OUT_LOC=${TEST_ROOT}/outputs

PMIMAGE=/mnt/pmem0/clht_pool
COPYFILE=${PMIMAGE}_flushed

BIN=example

TEST_MEMCPY_TYPE=RAND_MEMCPY

cd src

cp ${PMEMWRAP_ROOT}/libpmem.h ${TEST_ROOT}/include
cp ${PMEMWRAP_ROOT}/libpmemobj.h ${TEST_ROOT}/include

cd ${TEST_ROOT}
./mybuild.sh

rm ${PMIMAGE} ${COPYFILE}

export PMEMWRAP_ABORT=0
export PMEMWRAP_WRITECOUNTFILE=YES
export PMEMWRAP_MEMCPY=NO_MEMCPY
${TEST_ROOT}/build/${BIN} 10000 1
export PMEMWRAP_WRITECOUNTFILE=ADD
export PMEMWRAP_ABORTCOUNT_LOOP=18

rm ${PMIMAGE} ${COPYFILE}

OUTPUT_TEXT=${OUT_LOC}/${BIN}_${TEST_MEMCPY_TYPE}_output.txt
ABORT_TEXT=${OUT_LOC}/${BIN}_${TEST_MEMCPY_TYPE}_abort.txt
ERROR_TEXT=${OUT_LOC}/${BIN}_${TEST_MEMCPY_TYPE}_error.txt

echo "" > ${OUTPUT_TEXT}
echo "" > ${ABORT_TEXT}
echo "" > ${ERROR_TEXT}
# echo "" > ${OUT_LOC}/${BIN}_memcpy.txt

for i in `seq 100`
do
    echo "${i}" >> ${OUTPUT_TEXT}
    echo "${i}" >> ${ABORT_TEXT}
    echo "${i}" >> ${ERROR_TEXT}
    export PMEMWRAP_ABORT=1
    export PMEMWRAP_SEED=${i}
    export PMEMWRAP_MEMCPY=${TEST_MEMCPY_TYPE}
    ${TEST_ROOT}/build/${BIN} 10000 1 >> ${OUTPUT_TEXT} 2>> ${ABORT_TEXT}
    ${PMEMWRAP_ROOT}/PmemWrap_memcpy.out ${PMIMAGE} ${COPYFILE}
#  >> ${OUT_LOC}/${BIN}_memcpy.txt

    export PMEMWRAP_ABORT=0
    export PMEMWRAP_MEMCPY=NO_MEMCPY
    timeout -k 1 40 bash -c "${TEST_ROOT}/build/${BIN} 10000 1 >> ${OUTPUT_TEXT} 2>> ${ERROR_TEXT}" 2>>${ABORT_TEXT}
    echo "timeout $?" >> ${ABORT_TEXT}
    rm ${PMIMAGE} ${COPYFILE}
    
    echo "" >> ${OUTPUT_TEXT}
    echo "" >> ${ABORT_TEXT}
    echo "" >> ${ERROR_TEXT}
done

#rm ${TEST_ROOT}/include/libpmemobj.h
