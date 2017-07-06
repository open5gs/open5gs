import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';
import CloseIcon from 'react-icons/lib/md/close';

import { Modal, Tooltip, Heading, Text, LineSeparator } from 'components';

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
  display: block;
  margin: 0.5rem 1rem;

  height: 300px;
  ${media.mobile`
    height: calc(100vh - 16rem);
  `}

  overflow: scroll;
`

const View = ({ visible, disableOnClickOutside, subscriber, onEdit, onDelete, onHide }) => {
  const imsi = (subscriber || {}).imsi;
  const security = ((subscriber || {}).security || {});
  const ue_ambr = ((subscriber || {}).ue_ambr || {});
  const pdns = ((subscriber || {}).pdn || []);

  return (
    <Modal 
      visible={visible} 
      onOutside={onHide}
      disableOnClickOutside={disableOnClickOutside}>
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
          <Heading el='h3'>Security</Heading>
          <Text>K : {security.k}</Text>
          <Text>OP : {security.op}</Text>
          <Text>AMF : {security.amf}</Text>
          <LineSeparator horizontal />
          <Heading el='h3'>UE AMBR(Aggregate Maximum Bit Rate)</Heading>
          <Text>Max Requested Bandwidth UL : {ue_ambr.max_bandwidth_ul} (Kbps)</Text>
          <Text>Max Requested Bandwidth DL : {ue_ambr.max_bandwidth_dl} (Kbps)</Text>
          <LineSeparator horizontal />
          <Heading el='h3'>PDN</Heading>
          {pdns.map(pdn => 
            <div key={pdn.apn}>
              <Text>APN : {pdn.apn}</Text>
              <Text>QCI : {pdn.qos.qci}</Text>
              <Text>ARP : {pdn.qos.arp.priority_level}</Text>
              <Text>AMBR : {pdn.pdn_ambr.max_bandwidth_ul}/{pdn.pdn_ambr.max_bandwidth_ul}</Text>
            </div>
          )}
        </Body>
      </Wrapper>
    </Modal>
  )
}

export default View;