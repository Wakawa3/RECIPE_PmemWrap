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


echo "" > ${OUT_LOC}/${BIN}_output.txt
echo "" > ${OUT_LOC}/${BIN}_abort.txt
echo "" > ${OUT_LOC}/${BIN}_error.txt
# echo "" > ${OUT_LOC}/${BIN}_memcpy.txt

for i in `seq 100`
do
    echo "${i}" >> ${OUT_LOC}/${BIN}_output.txt
    echo "${i}" >> ${OUT_LOC}/${BIN}_abort.txt
    echo "${i}" >> ${OUT_LOC}/${BIN}_error.txt
    export PMEMWRAP_ABORT=1
    export PMEMWRAP_SEED=${i}
    export PMEMWRAP_MEMCPY=NORMAL_MEMCPY
    ${TEST_ROOT}/build/${BIN} 10000 1 >> ${OUT_LOC}/${BIN}_output.txt 2>> ${OUT_LOC}/${BIN}_abort.txt
    ${PMEMWRAP_ROOT}/PmemWrap_memcpy.out ${PMIMAGE} ${COPYFILE}
#  >> ${OUT_LOC}/${BIN}_memcpy.txt

    export PMEMWRAP_ABORT=0
    export PMEMWRAP_MEMCPY=NO_MEMCPY
    timeout -k 1 40 bash -c "${TEST_ROOT}/build/${BIN} 10000 1 >> ${OUT_LOC}/${BIN}_output.txt 2>> ${OUT_LOC}/${BIN}_error.txt" 2>>${OUT_LOC}/${BIN}_abort.txt
    echo "timeout $?" >> ${OUT_LOC}/${BIN}_abort.txt
    rm ${PMIMAGE} ${COPYFILE}
    
    echo "" >> ${OUT_LOC}/${BIN}_output.txt
    echo "" >> ${OUT_LOC}/${BIN}_abort.txt
    echo "" >> ${OUT_LOC}/${BIN}_error.txt
done

#rm ${TEST_ROOT}/include/libpmemobj.h
