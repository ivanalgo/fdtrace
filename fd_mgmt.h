#ifndef _FD_MGMT_H
#define _FD_MGMT_H

int mgmt_renew_empty_fd(int old_fd);
void mgmt_close_fd(int fd);
void mgmt_access_fd(int fd);

#endif /* _FD_MGMT_H */
