/*
 * ListToolbar — search box + pagination controls for large CHF collections.
 *
 * Presentational and fully controlled: all state (search text, current page)
 * lives in the parent container, which also owns the server-side query params
 * (?limit, ?skip, ?sort, ?query).  This component only renders controls and
 * calls back on user intent, so it stays reusable across the CHF subscriber
 * and session views.
 *
 * Scale rationale: the parent pages the collection server-side (a bounded
 * ?limit per request) and this toolbar drives skip/query, so the browser
 * never holds or renders more than one page regardless of how many
 * subscribers or active sessions exist.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

import PropTypes from 'prop-types';
import styled from 'styled-components';
import oc from 'open-color';

import { media } from 'helpers/style-utils';

const Bar = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  margin: 1.5rem 2rem 0 2rem;
  gap: 1rem;

  ${media.mobile`
    flex-direction: column;
    align-items: stretch;
    margin: 0.75rem 0.5rem 0 0.5rem;
  `}
`;

const Search = styled.input`
  flex: 1;
  max-width: 420px;
  padding: 0.5rem 0.75rem;
  border: 1px solid ${oc.gray[4]};
  border-radius: 4px;
  font-size: 0.9rem;
  color: ${oc.gray[8]};

  &:focus {
    outline: none;
    border-color: ${oc.indigo[5]};
  }
`;

const Pager = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
  white-space: nowrap;
`;

const Count = styled.span`
  font-size: 0.85rem;
  color: ${oc.gray[6]};
  margin-right: 1rem;
`;

const Button = styled.button`
  padding: 0.4rem 0.75rem;
  margin-left: 0.25rem;
  border: 1px solid ${oc.gray[4]};
  border-radius: 4px;
  background: ${props => (props.disabled ? oc.gray[1] : oc.white)};
  color: ${props => (props.disabled ? oc.gray[4] : oc.indigo[7])};
  font-size: 0.85rem;
  cursor: ${props => (props.disabled ? 'default' : 'pointer')};

  &:hover {
    background: ${props => (props.disabled ? oc.gray[1] : oc.gray[0])};
  }
`;

/*
 * Build the "X–Y of N" label.  When total is unknown (header absent) fall
 * back to a page-relative label so the control still works.
 */
function rangeLabel(page, pageSize, count, total) {
  const start = count > 0 ? page * pageSize + 1 : 0;
  const end = page * pageSize + count;
  if (typeof total === 'number' && total >= 0)
    return `${start}–${end} of ${total}`;
  return `${start}–${end}`;
}

const ListToolbar = ({
  showSearch,
  search,
  searchPlaceholder,
  onSearchChange,
  page,
  pageSize,
  count,
  total,
  onPrev,
  onNext,
}) => {
  const hasPrev = page > 0;
  /*
   * "Next" is enabled when we know there are more (total > shown) or, when
   * total is unknown, when this page came back full (a full page implies
   * there may be another).
   */
  const shownSoFar = page * pageSize + count;
  const hasNext =
    typeof total === 'number' ? shownSoFar < total : count >= pageSize;

  return (
    <Bar>
      {showSearch ? (
        <Search
          type="text"
          value={search}
          placeholder={searchPlaceholder}
          onChange={e => onSearchChange(e.target.value)}
        />
      ) : (
        <span />
      )}
      <Pager>
        <Count>{rangeLabel(page, pageSize, count, total)}</Count>
        <Button disabled={!hasPrev} onClick={() => hasPrev && onPrev()}>
          Prev
        </Button>
        <Button disabled={!hasNext} onClick={() => hasNext && onNext()}>
          Next
        </Button>
      </Pager>
    </Bar>
  );
};

ListToolbar.propTypes = {
  showSearch: PropTypes.bool,
  search: PropTypes.string,
  searchPlaceholder: PropTypes.string,
  onSearchChange: PropTypes.func,
  page: PropTypes.number.isRequired,
  pageSize: PropTypes.number.isRequired,
  count: PropTypes.number.isRequired,
  total: PropTypes.number,
  onPrev: PropTypes.func.isRequired,
  onNext: PropTypes.func.isRequired,
};

ListToolbar.defaultProps = {
  showSearch: false,
  search: '',
  searchPlaceholder: 'Search',
  onSearchChange: () => {},
  total: undefined,
};

export default ListToolbar;
