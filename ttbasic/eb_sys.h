#ifdef __cplusplus
extern "C" {
#endif

void eb_wait(unsigned int ms);
unsigned int eb_tick(void);
void eb_process_events(void);

#ifdef __cplusplus
}
#endif
