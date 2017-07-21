export const NOTIFICATION = {
  SHOW: 'notification/SHOW',
  HIDE: 'notification/HIDE',
  CLEAR: 'notification/CLEAR',
}

export function show(opts = {}, level = 'success') {
  return {
    type: NOTIFICATION.SHOW,
    ...opts,
    uid: opts.uid || Date.now(),
    position: opts.position || 'bc',
    autoDismiss: opts.autoDismiss === undefined ?  2 : opts.autoDismiss,
    level
  };
}

export function success(opts) {
  return show(opts, 'success');
}

export function error(opts) {
  return show(opts, 'error');
}

export function warning(opts) {
  return show(opts, 'warning');
}

export function info(opts) {
  return show(opts, 'info');
}

export function hide(uid) {
  return {
    type: NOTIFICATION.HIDE,
    uid
  };
}

export function clear() {
  return { type: NOTIFICATION.CLEAR };
}
