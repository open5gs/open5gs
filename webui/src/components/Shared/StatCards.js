/*
 * StatCards — a row of summary metric tiles.
 *
 * Used as the default CHF landing view so operators see counts (online
 * sessions, provisioned subscribers, recent activity) without the WebUI
 * downloading or rendering any collection.  Purely presentational; the
 * container supplies already-computed values (typically from X-Total-Count).
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

import PropTypes from 'prop-types';
import styled from 'styled-components';
import oc from 'open-color';

import { media } from 'helpers/style-utils';

const Row = styled.div`
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;
  gap: 1rem;
  margin: 2rem;

  ${media.mobile`
    margin: 1rem 0.5rem;
  `}
`;

const Card = styled.div`
  flex: 1;
  min-width: 160px;
  padding: 1.25rem 1.5rem;
  background: ${oc.white};
  border: 1px solid ${oc.gray[3]};
  border-radius: 6px;
  box-shadow: 0 1px 2px rgba(0, 0, 0, 0.04);
`;

const Value = styled.div`
  font-size: 2rem;
  font-weight: 700;
  color: ${oc.indigo[7]};
  line-height: 1.1;
`;

const Label = styled.div`
  margin-top: 0.4rem;
  font-size: 0.8rem;
  font-weight: bold;
  text-transform: uppercase;
  letter-spacing: 0.03em;
  color: ${oc.gray[6]};
`;

const Hint = styled.div`
  margin-top: 0.25rem;
  font-size: 0.75rem;
  color: ${oc.gray[5]};
`;

const fmt = (v) => {
  if (v === null || v === undefined) return '—';
  if (typeof v === 'number') return v.toLocaleString();
  return String(v);
};

const StatCards = ({ stats }) => (
  <Row>
    {stats.map((s, i) => (
      <Card key={s.label || i}>
        <Value>{fmt(s.value)}</Value>
        <Label>{s.label}</Label>
        {s.hint ? <Hint>{s.hint}</Hint> : null}
      </Card>
    ))}
  </Row>
);

StatCards.propTypes = {
  stats: PropTypes.arrayOf(
    PropTypes.shape({
      label: PropTypes.string.isRequired,
      value: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
      hint: PropTypes.string,
    })
  ).isRequired,
};

export default StatCards;
