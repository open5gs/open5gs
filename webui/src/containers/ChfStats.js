/*
 * CHF summary statistics header.
 *
 * The default landing view for both CHF pages: shows counts (provisioned
 * subscribers, online sessions, sessions active in the recent window) without
 * downloading any collection.  Each count is a cheap limit:1 fetch whose
 * X-Total-Count header carries the real total, so the cost is one
 * countDocuments per stat regardless of collection size.
 *
 * Refreshes on a slow, visibility-aware interval so a backgrounded tab does
 * not keep polling.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchChfSubscribers } from 'modules/crud/chf-subscriber';
import { fetchChfSessions } from 'modules/crud/chf-session';
import { select } from 'modules/crud/selectors';
import { countParams, totalFromOtherInfo } from 'helpers/chf-query';

import { StatCards } from 'components';

const REFRESH_MS = 15000;

/* Unfiltered counts have no time component, so hoist them: one object, one
 * crud cache key.  The windowed count is built per render but is equally
 * stable — windowIso() snaps its bound to a bucket (see helpers/chf-query). */
const SUBS_COUNT = countParams('subscriber', '', 0);
const SESS_COUNT = countParams('session', '', 0);

class ChfStats extends Component {
  componentDidMount() {
    if (typeof document !== 'undefined') {
      document.addEventListener('visibilitychange', this.handleVisibility);
    }
    this.refresh();
    this.start();
  }

  componentWillReceiveProps(nextProps) {
    /* Fill a count the selector has nothing cached for, rather than leaving
     * the card blank until the next tick — this covers a window-selector
     * change and the minute rollover of the windowed bucket.  It terminates
     * because the params are stable: once the fetch lands, needsFetch stays
     * false for that key. */
    ['subs', 'sessions', 'recent'].forEach(k => {
      if (nextProps[k] && nextProps[k].needsFetch) {
        this.props.dispatch(nextProps[k].fetch);
      }
    });
  }

  componentWillUnmount() {
    if (typeof document !== 'undefined') {
      document.removeEventListener('visibilitychange', this.handleVisibility);
    }
    this.stop();
  }

  recentParams() {
    return countParams('session', '', this.props.windowHours);
  }

  refresh() {
    this.props.dispatch(fetchChfSubscribers(SUBS_COUNT));
    this.props.dispatch(fetchChfSessions(SESS_COUNT));
    this.props.dispatch(fetchChfSessions(this.recentParams()));
  }

  start() {
    this.stop();
    if (typeof document !== 'undefined' && document.hidden) return;
    this._timer = setInterval(() => this.refresh(), REFRESH_MS);
  }

  stop() {
    if (this._timer) { clearInterval(this._timer); this._timer = null; }
  }

  handleVisibility = () => {
    if (document.hidden) this.stop();
    else { this.refresh(); this.start(); }
  };

  render() {
    const { subs, sessions, recent, windowHours } = this.props;
    const stats = [
      {
        label: 'Provisioned Subscribers',
        value: totalFromOtherInfo(subs.otherInfo),
        hint: 'CHF quota profiles',
      },
      {
        label: 'Online Sessions',
        value: totalFromOtherInfo(sessions.otherInfo),
        hint: 'active charging sessions',
      },
      {
        label: `Active (last ${windowHours}h)`,
        value: totalFromOtherInfo(recent.otherInfo),
        hint: 'sessions started in window',
      },
    ];
    return <StatCards stats={stats} />;
  }
}

ChfStats.propTypes = {
  dispatch: PropTypes.func.isRequired,
  windowHours: PropTypes.number,
  subs: PropTypes.object.isRequired,
  sessions: PropTypes.object.isRequired,
  recent: PropTypes.object.isRequired,
};

ChfStats.defaultProps = {
  windowHours: 1,
};

export default connect((state, ownProps) => ({
  subs: select(fetchChfSubscribers(SUBS_COUNT), state.crud),
  sessions: select(fetchChfSessions(SESS_COUNT), state.crud),
  recent: select(
    fetchChfSessions(countParams('session', '', ownProps.windowHours || 1)),
    state.crud
  ),
}))(ChfStats);
