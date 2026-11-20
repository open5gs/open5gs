/*
 * CHF active-session monitor (read-only).
 *
 * Scale hardening for large live-session sets:
 *   - Server-side pagination: only one page (?limit/?skip, newest first) is
 *     fetched and rendered, never the whole chf.sessions collection.
 *   - Optional SUPI filter (?query) to focus on one subscriber.
 *   - Visibility-aware polling: the 10s refresh runs only while the tab is
 *     visible, and refreshes immediately on return — a backgrounded monitor
 *     stops hammering the API and the browser.
 *
 * Structure: a stateful parent owns page/search + the poll timer; a connected
 * child selects exactly the page named by queryParams (so a page/filter change
 * flips the crud "needsFetch" flag and re-fetches through the normal path).
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchChfSessions } from 'modules/crud/chf-session';
import { select } from 'modules/crud/selectors';
import { buildSessionParams, totalFromOtherInfo } from 'helpers/chf-query';

import { Layout, ChfSession, Spinner, Blank, ListToolbar } from 'components';
import ChfStats from 'containers/ChfStats';

const POLL_INTERVAL_MS = 10000;
const PAGE_SIZE = 25;
const SEARCH_DEBOUNCE_MS = 400;
/* Activity windows offered in the monitor (hours; 0 = all). */
const WINDOW_OPTIONS = [
  { label: 'Last 1 hour', hours: 1 },
  { label: 'Last 6 hours', hours: 6 },
  { label: 'Last 24 hours', hours: 24 },
  { label: 'All active', hours: 0 },
];

/* ------------------------------------------------------------------ */
/* Connected data view — selects and renders one page of sessions.     */
/* ------------------------------------------------------------------ */
class SessionData extends Component {
  componentWillMount() {
    this.maybeFetch(this.props);
  }

  componentWillReceiveProps(nextProps) {
    this.maybeFetch(nextProps);
  }

  maybeFetch(props) {
    if (props.chfSessions.needsFetch) {
      props.dispatch(props.chfSessions.fetch);
    }
  }

  render() {
    const {
      chfSessions, page, pageSize, search,
      onSearchChange, onPrev, onNext,
    } = this.props;
    const { isLoading, data, otherInfo } = chfSessions;
    const total = totalFromOtherInfo(otherInfo);

    return (
      <div>
        <ListToolbar
          showSearch
          search={search}
          searchPlaceholder="Filter by SUPI (e.g. imsi-999700000000001)"
          onSearchChange={onSearchChange}
          page={page}
          pageSize={pageSize}
          count={data.length}
          total={total}
          onPrev={onPrev}
          onNext={onNext}
        />
        <ChfSession.List sessions={data} />
        {isLoading && <Spinner md />}
        <Blank
          visible={!isLoading && !(data.length > 0)}
          title="NO ACTIVE CHF SESSIONS"
          body="No active charging sessions match this view."
        />
      </div>
    );
  }
}

SessionData.propTypes = {
  chfSessions: PropTypes.object.isRequired,
  dispatch: PropTypes.func.isRequired,
  page: PropTypes.number.isRequired,
  pageSize: PropTypes.number.isRequired,
  search: PropTypes.string.isRequired,
  onSearchChange: PropTypes.func.isRequired,
  onPrev: PropTypes.func.isRequired,
  onNext: PropTypes.func.isRequired,
};

const ConnectedSessionData = connect((state, ownProps) => ({
  chfSessions: select(fetchChfSessions(ownProps.queryParams), state.crud),
}))(SessionData);

/* ------------------------------------------------------------------ */
/* Stateful parent — owns page/search and the visibility-aware poll.   */
/* ------------------------------------------------------------------ */
class Collection extends Component {
  state = {
    page: 0,
    searchInput: '',   // controlled input value (updates every keystroke)
    searchApplied: '', // debounced value that actually drives the query
    windowHours: 1,    // default: sessions activated in the last hour
    queryParams: null, // params of the page currently displayed (see poll)
  };

  componentDidMount() {
    if (typeof document !== 'undefined') {
      document.addEventListener('visibilitychange', this.handleVisibility);
    }
    this.poll();
    this.startPolling();
  }

  componentDidUpdate(prevProps, prevState) {
    /* Page / filter / window changed — re-query immediately rather than
     * waiting for the next tick.  poll() only writes queryParams, so this
     * does not re-enter. */
    if (prevState.page !== this.state.page ||
        prevState.searchApplied !== this.state.searchApplied ||
        prevState.windowHours !== this.state.windowHours) {
      this.poll();
    }
  }

  componentWillUnmount() {
    if (typeof document !== 'undefined') {
      document.removeEventListener('visibilitychange', this.handleVisibility);
    }
    this.stopPolling();
    if (this._searchTimer) clearTimeout(this._searchTimer);
  }

  params() {
    return buildSessionParams(
      this.state.page, PAGE_SIZE, this.state.searchApplied,
      this.state.windowHours);
  }

  /*
   * Fetch the current page and hand the child the exact params it was
   * fetched with.
   *
   * The window bound advances once a minute (windowIso buckets it), so
   * recomputing params independently in the poll and in render would let
   * the two drift apart at a bucket boundary: the poll would write results
   * under a key the mounted child never selects, and the child would stay
   * anchored to the bucket it first rendered with.  Routing both through
   * state keeps them identical by construction.
   */
  poll = () => {
    const queryParams = this.params();
    this.setState({ queryParams });
    this.props.dispatch(fetchChfSessions(queryParams));
  };

  handleWindowChange = (e) => {
    const hours = parseInt(e.target.value, 10) || 0;
    this.setState({ windowHours: hours, page: 0 });
  };

  startPolling() {
    this.stopPolling();
    /* Do not poll a hidden tab — it would download + re-render a page of
     * sessions every interval with nobody watching. */
    if (typeof document !== 'undefined' && document.hidden) return;
    this._timer = setInterval(this.poll, POLL_INTERVAL_MS);
  }

  stopPolling() {
    if (this._timer) {
      clearInterval(this._timer);
      this._timer = null;
    }
  }

  handleVisibility = () => {
    if (document.hidden) {
      this.stopPolling();
    } else {
      /* Refresh immediately on return, then resume the timer. */
      this.poll();
      this.startPolling();
    }
  };

  handleSearchChange = (value) => {
    this.setState({ searchInput: value });
    if (this._searchTimer) clearTimeout(this._searchTimer);
    /* Debounce: apply (and thus re-query) only after the user pauses, so we
     * do not fire a fetch per keystroke. */
    this._searchTimer = setTimeout(() => {
      this.setState({ searchApplied: value.trim(), page: 0 });
    }, SEARCH_DEBOUNCE_MS);
  };

  handlePrev = () => this.setState(s => ({ page: Math.max(0, s.page - 1) }));
  handleNext = () => this.setState(s => ({ page: s.page + 1 }));

  render() {
    /* Changing page/searchApplied/window changes queryParams -> the child's
     * selector reports needsFetch -> it fetches that page. */
    const windowSelect = (
      <select
        value={this.state.windowHours}
        onChange={this.handleWindowChange}
        style={{
          margin: '1rem 2rem 0 2rem',
          padding: '0.4rem 0.6rem',
          border: '1px solid #ced4da',
          borderRadius: 4,
          fontSize: '0.85rem',
        }}
      >
        {WINDOW_OPTIONS.map(o => (
          <option key={o.hours} value={o.hours}>{o.label}</option>
        ))}
      </select>
    );

    return (
      <Layout.Content>
        <ChfStats windowHours={this.state.windowHours || 1} />
        {windowSelect}
        <ConnectedSessionData
          queryParams={this.state.queryParams || this.params()}
          page={this.state.page}
          pageSize={PAGE_SIZE}
          search={this.state.searchInput}
          dispatch={this.props.dispatch}
          onSearchChange={this.handleSearchChange}
          onPrev={this.handlePrev}
          onNext={this.handleNext}
        />
      </Layout.Content>
    );
  }
}

Collection.propTypes = {
  dispatch: PropTypes.func.isRequired,
};

export default connect()(Collection);
