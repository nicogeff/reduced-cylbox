
#ifndef NAL_RTOS_MUTEXES_TESTS_H
#define NAL_RTOS_MUTEXES_TESTS_H

#ifdef __cplusplus
extern "C"
{
#endif

void nal_test_mutexes_simpleUsage(void);
void nal_test_mutexes_createTaskMixingMutex(void);
void nal_test_mutexes_doubleInitializing(void);
void nal_test_mutexes_doubleDeleting(void);
void nal_test_mutexes_deleteMutexLockByTask3(void);
void nal_test_mutexes_GetLockedMutexByMain(void);
void nal_test_mutexes_callingDeletedMutex(void);
void nal_test_mutexes_defensiveCode(void);

#ifdef __cplusplus
}
#endif

#endif /* NAL_RTOS_MUTEXES_TESTS_H */

