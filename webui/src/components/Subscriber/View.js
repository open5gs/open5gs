import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';
import CloseIcon from 'react-icons/lib/md/close';

import { Modal, Tooltip } from 'components';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  postion: relative;
  width: 600px;

  ${media.mobile`
    width: calc(100vw - 4rem);
  `}

  background: white;
  box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
`

const Header = styled.div`
  position: relative;
  display: flex;

  background: ${oc.gray[1]};

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

const Imsi = styled.div`
  padding: 1.5rem;
  color: ${oc.gray[8]};
  font-size: 1.5rem;
`;

const CircleButton = styled.div`
  height: 2rem;
  width: 2rem;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 1px;

  ${media.mobile`
    margin: 0px;
  `}

  color: ${oc.gray[6]};

  border-radius: 1rem;
  font-size: 1.5rem;

  &:hover {
    color: ${oc.indigo[6]};
  }

  &.delete {
    &:hover {
      color: ${oc.pink[6]};
    }
  }
`

const Body = styled.div`
  padding: 2rem;
  font-size: 14px;

  height: 300px;
  ${media.mobile`
    height: calc(100vh - 16rem);
  `}

  overflow: scroll;
`

const View = ({ visible, subscriber, onEdit, onDelete, onHide }) => {
  const imsi = (subscriber || {}).imsi;

  return (
    <Modal 
      visible={visible} 
      onOutside={onHide}>
      <Wrapper>
        <Header>
          <Imsi>{imsi}</Imsi>
          <div className="actions">
            <Tooltip content='Edit' width="60px">
              <CircleButton onClick={() => onEdit(imsi)}><EditIcon/></CircleButton>
            </Tooltip>
            <Tooltip content='Delete' width="60px">
              <CircleButton className="delete" onClick={() => onDelete(imsi)}><DeleteIcon/></CircleButton>
            </Tooltip>
            <Tooltip content='Close' width="60px">
              <CircleButton className="delete" onClick={onHide}><CloseIcon/></CircleButton>
            </Tooltip>
          </div>
        </Header>
        <Body>
        skldafjasdfjalskdf
        </Body>
      </Wrapper>
    </Modal>
  )
}

export default View;