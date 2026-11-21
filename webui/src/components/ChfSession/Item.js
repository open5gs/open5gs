import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import { BYTES_PER_MB } from 'helpers/chf-quota-convert';

const Row = styled.div`
  display: flex;
  flex-direction: row;
  flex: 1;
  padding: 0.5rem 2.5rem;
  border-bottom: 1px solid ${oc.gray[2]};
  line-height: 2rem;

  &:hover {
    background: ${oc.gray[1]};
  }

  .ref {
    font-size: 0.95rem;
    color: ${oc.gray[8]};
    width: 220px;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }
  .supi {
    font-size: 0.95rem;
    color: ${oc.gray[7]};
    width: 220px;
  }
  .dnn {
    font-size: 0.95rem;
    color: ${oc.gray[6]};
    width: 120px;
  }
  .octets {
    font-size: 0.95rem;
    color: ${oc.gray[6]};
    width: 120px;
    text-align: right;
    padding-right: 1rem;
  }
  .start {
    font-size: 0.85rem;
    color: ${oc.gray[5]};
    width: 180px;
  }
`;

/* Fixed unit (not the auto-scaling fmtVolume) so the UL/DL columns stay
 * visually comparable down the list. */
function fmtMiB(bytes) {
  if (bytes == null) return '—';
  return (bytes / BYTES_PER_MB).toFixed(2) + ' MiB';
}

function fmtDate(d) {
  if (!d) return '—';
  try {
    return new Date(d).toLocaleString();
  } catch (e) {
    return String(d);
  }
}

const Item = ({ session }) => (
  <Row>
    <div className="ref">{session.chargingDataRef}</div>
    <div className="supi">{session.supi}</div>
    <div className="dnn">{session.dnn || '—'}</div>
    <div className="octets">{fmtMiB(session.ulOctets)}</div>
    <div className="octets">{fmtMiB(session.dlOctets)}</div>
    <div className="start">{fmtDate(session.startTime)}</div>
  </Row>
);

Item.propTypes = {
  session: PropTypes.shape({
    chargingDataRef: PropTypes.string,
    supi: PropTypes.string,
    dnn: PropTypes.string,
    ulOctets: PropTypes.number,
    dlOctets: PropTypes.number,
    startTime: PropTypes.oneOfType([PropTypes.string, PropTypes.instanceOf(Date)]),
  }),
};

export default Item;
