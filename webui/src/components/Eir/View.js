import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';
import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';
import CloseIcon from 'react-icons/lib/md/close';
import SecurityIcon from 'react-icons/lib/md/security';
import PhoneIcon from 'react-icons/lib/md/phone';
import KeyboardControlIcon from 'react-icons/lib/md/keyboard-control';
import { Modal, Tooltip } from 'components';
const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  position: relative;
  width: 700px;
  ${media.mobile`width:calc(100vw - 4rem);`}background:white;
  box-shadow: 0 10px 20px rgba(0, 0, 0, 0.19), 0 6px 6px rgba(0, 0, 0, 0.23);
`;
const Header = styled.div`
  position: relative;
  display: flex;
  background: ${oc.gray[1]};
  .title {
    padding: 1.5rem;
    color: ${oc.gray[8]};
    font-size: 1.5rem;
  }
  .actions {
    position: absolute;
    top: 0;
    right: 0;
    width: 8rem;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
  }
`;
const CircleButton = styled.div`
  height: 2rem;
  width: 2rem;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 1px;
  color: ${oc.gray[6]};
  border-radius: 1rem;
  font-size: 1.5rem;
  &:hover {
    color: ${oc.indigo[6]};
  }
  &.delete:hover {
    color: ${oc.pink[6]};
  }
`;
const Body = styled.div`
  display: block;
  margin: 0.5rem;
  min-height: 190px;
  overflow: scroll;
`;
const Details = styled.div`
  display: flex;
  flex-direction: column;
  margin: 0 auto;
  color: ${oc.gray[9]};
  .header {
    margin: 12px;
    font-size: 16px;
  }
  .body {
    display: flex;
    flex-direction: row;
    flex: 1;
    margin: 6px;
  }
  .left {
    width: 80px;
    text-align: center;
    font-size: 18px;
    color: ${oc.gray[6]};
  }
  .right {
    display: flex;
    flex-direction: column;
    flex: 1;
  }
  .data {
    flex: 1;
    font-size: 12px;
    margin: 4px;
  }
`;
const raw = (value) => (value || '').replace(/^(imei|imeisv|imsi)-/, '');
export default ({
  visible,
  disableOnClickOutside,
  record,
  onEdit,
  onDelete,
  onHide
}) => {
  const r = record || {};
  return (
    <Modal
      visible={visible}
      onOutside={onHide}
      disableOnClickOutside={disableOnClickOutside}
    >
      <Wrapper>
        <Header>
          <div className="title">{raw(r.pei)}</div>
          <div className="actions">
            <Tooltip content="Edit" width="60px">
              <CircleButton onClick={() => onEdit(r._id)}>
                <EditIcon />
              </CircleButton>
            </Tooltip>
            <Tooltip content="Delete" width="60px">
              <CircleButton className="delete" onClick={() => onDelete(r._id)}>
                <DeleteIcon />
              </CircleButton>
            </Tooltip>
            <Tooltip content="Close" width="60px">
              <CircleButton className="delete" onClick={onHide}>
                <CloseIcon />
              </CircleButton>
            </Tooltip>
          </div>
        </Header>
        <Body>
          <Details>
            <div className="header">Equipment Identity Rule</div>
            <div className="body">
              <div className="left">
                <PhoneIcon />
              </div>
              <div className="right">
                <div className="data">
                  {(r.pei || '').split('-')[0].toUpperCase()}
                  <span style={{ color: oc.gray[5] }}>
                    <KeyboardControlIcon />
                    Identity Type
                  </span>
                </div>
                <div className="data">
                  {raw(r.pei)}
                  <span style={{ color: oc.gray[5] }}>
                    <KeyboardControlIcon />
                    IMEI / IMEISV
                  </span>
                </div>
                {r.supi && (
                  <div className="data">
                    {raw(r.supi)}
                    <span style={{ color: oc.gray[5] }}>
                      <KeyboardControlIcon />
                      IMSI
                    </span>
                  </div>
                )}
              </div>
            </div>
            <div className="body">
              <div className="left">
                <SecurityIcon />
              </div>
              <div className="right">
                <div className="data">
                  {r.supi ? 'Subscriber Rule' : 'Device Rule'}
                  <span style={{ color: oc.gray[5] }}>
                    <KeyboardControlIcon />
                    Rule Type
                  </span>
                </div>
                <div className="data">
                  {r.status}
                  <span style={{ color: oc.gray[5] }}>
                    <KeyboardControlIcon />
                    Equipment Status
                  </span>
                </div>
              </div>
            </div>
          </Details>
        </Body>
      </Wrapper>
    </Modal>
  );
};
