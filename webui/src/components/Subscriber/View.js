import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';
import CloseIcon from 'react-icons/lib/md/close';

import PhoneIcon from 'react-icons/lib/md/phone';
import SecurityIcon from 'react-icons/lib/md/security';
import PdnIcon from 'react-icons/lib/md/cast';
import KeyboardControlIcon from 'react-icons/lib/md/keyboard-control';

import { Modal, Tooltip, Dimmed } from 'components';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  postion: relative;
  width: 900px;

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

  &.delete {
    &:hover {
      color: ${oc.pink[6]};
    }
  }
`

const Body = styled.div`
  display: block;
  margin: 0.5rem;

  height: 500px;
  ${media.mobile`
    height: calc(100vh - 16rem);
  `}

  overflow: scroll;
`

const Subscriber = styled.div`
  display: flex;
  flex-direction: column;
  margin: 0, auto;
  color: ${oc.gray[9]};

  .header {
    margin: 12px;
    font-size: 16px;
  }
  .body {
    display: flex;
    flex-direction: row;
    flex:1;
    margin: 6px;

    .left {
      width: 80px;
      text-align: center;
      font-size: 18px;
      color: ${oc.gray[6]};
    }

    .right {
      display: flex;
      flex-direction: column;
      flex:1;

      .data {
        flex:1;
        font-size: 12px;
        margin: 4px;
      }
    }
  }
`

const Pdn = styled.div`
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
    flex:1;
    margin: 0px 32px;

    .small_data {
      width: 50px;
      font-size: 12px;
      margin: 4px;
    }
    .medium_data {
      width: 80px;
      font-size: 12px;
      margin: 4px;
    }
    .large_data {
      width: 140px;
      font-size: 12px;
      margin: 4px;
    }
  }
`
const View = ({ visible, disableOnClickOutside, subscriber, onEdit, onDelete, onHide }) => {
  const imsi = (subscriber || {}).imsi;
  const msisdn_list = ((subscriber || {}).msisdn || []);
  const imeisv = (subscriber || {}).imeisv;
  const security = ((subscriber || {}).security || {});
  const ambr = ((subscriber || {}).ambr || {});
  const slice_list = ((subscriber || {}).slice || []);

  return (
    <div>
      <Modal
        visible={visible}
        onOutside={onHide}
        disableOnClickOutside={disableOnClickOutside}>
        <Wrapper>
          <Header>
            <div className="title">{imsi}</div>
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
            <Subscriber>
              <div className="header">
                Subscriber Configuration
              </div>
              {(msisdn_list.length !== 0 || (imeisv && imeisv.length !== 0)) &&
                <div className="body">
                  <div className="left">
                    <PhoneIcon/>
                  </div>
                  <div className="right">
                    {msisdn_list.map((msisdn, index) =>
                      <div key={index} className="data">
                        {msisdn}
                        <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>MSISDN</span>
                      </div>
                    )}
                    {imeisv && imeisv.length !== 0 &&
                      <div className="data">
                        {imeisv}
                        <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>IMEISV</span>
                      </div>
                    }
                  </div>
                </div>
              }
              <div className="body">
                <div className="left">
                  <SecurityIcon/>
                </div>
                <div className="right">
                  <div className="data">
                    {security.k}
                    <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>K</span>
                  </div>
                  {security.opc &&
                    <div className="data">
                      {security.opc}
                      <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>OPc</span>
                    </div>
                  }
                  {security.op &&
                    <div className="data">
                      {security.op}
                      <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>OP</span>
                    </div>
                  }
                  <div className="data">
                    {security.amf}
                    <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>AMF</span>
                  </div>
                  {security.sqn &&
                    <div className="data">
                      {security.sqn}
                      <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>SQN</span>
                    </div>
                  }
                </div>
              </div>
              <div className="body">
                <div className="left">
                  <PdnIcon/>
                </div>
                <div className="right">
                  <div className="data">
                    {ambr['downlink'] === undefined ? "unlimited" :
                      ambr.downlink['value'] === undefined ? "unlimited" :
                        ambr.downlink.value
                    } {ambr['downlink'] === undefined ? "unlimited" :
                         ambr.downlink['value'] === undefined ? "" :
                         ambr.downlink['unit'] === undefined ? "bps" :
                            ambr.downlink.unit === 0 ? "bps" :
                            ambr.downlink.unit === 1 ? "Kbps" :
                            ambr.downlink.unit === 2 ? "Mbps" :
                            ambr.downlink.unit === 3 ? "Gbps" :
                            ambr.downlink.unit === 4 ? "Tbps" :
                              "Unknown Unit" }
                    <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>DL</span>
                  </div>
                  <div className="data">
                    {ambr['uplink'] === undefined ? "unlimited" :
                      ambr.uplink['value'] === undefined ? "unlimited" :
                        ambr.uplink.value
                    } {ambr['uplink'] === undefined ? "unlimited" :
                         ambr.uplink['value'] === undefined ? "" :
                         ambr.uplink['unit'] === undefined ? "bps" :
                            ambr.uplink.unit === 0 ? "bps" :
                            ambr.uplink.unit === 1 ? "Kbps" :
                            ambr.uplink.unit === 2 ? "Mbps" :
                            ambr.uplink.unit === 3 ? "Gbps" :
                            ambr.uplink.unit === 4 ? "Tbps" :
                              "Unknown Unit" }
                    <span style={{color:oc.gray[5]}}><KeyboardControlIcon/>UL</span>
                  </div>
                </div>
              </div>
            </Subscriber>
            <Pdn>
              {slice_list.map((slice, index) =>
                <div key={index}>
                  {slice.sd === undefined ?
                    <div className="header">
                      SST:{slice.sst} {slice.default_indicator ==
                              true ? "(Default S-NSSAI)" : ""}
                    </div> :
                    <div className="header">
                      SST:{slice.sst} SD:{slice.sd} {slice.default_indicator ==
                              true ? "(Default S-NSSAI)" : ""}
                    </div>
                  }
                  <div className="body" style={{color:oc.gray[5]}}>
                    <div className="medium_data">DNN/APN</div>
                    <div className="medium_data">Type</div>
                    <div className="small_data">5QI/QCI</div>
                    <div className="small_data">ARP</div>
                    <div className="medium_data">Capability</div>
                    <div className="medium_data">Vulnerablility</div>
                    <div className="large_data">MBR DL/UL</div>
                    <div className="large_data">GBR DL/UL</div>
                  </div>
                  {slice['session'] !== undefined &&
                      slice.session.map(session =>
                    <div key={session.name}>
                      <div className="body">
                        <div className="medium_data">{session.name}</div>
                        <div className="medium_data">{
                            session.type === 1 ? "IPv4" :
                            session.type === 2 ? "IPv6" :
                            session.type === 3 ? "IPv4v6" :
                            "Unknown"
                        }</div>
                        <div className="small_data">{session.qos.index}</div>
                        <div className="small_data">{session.qos.arp.priority_level}</div>
                        <div className="medium_data">
                          {session.qos.arp.pre_emption_capability ===
                              2 ? "Enabled" :
                           session.qos.arp.pre_emption_capability ===
                              1 ? "Disabled" : "Unknown"}
                        </div>
                        <div className="medium_data">
                          {session.qos.arp.pre_emption_vulnerability ===
                              2 ? "Enabled" :
                           session.qos.arp.pre_emption_vulnerability ===
                              1 ? "Disabled" : "Unknown"}
                        </div>
                        {session['ambr'] === undefined ?
                          <div className="large_data">
                            unlimited/unlimited
                          </div> :
                          <div className="large_data">
                            {session.ambr['downlink'] === undefined ? "unlimited" :
                              session.ambr.downlink['value'] === undefined ?
                                "unlimited" : session.ambr.downlink.value
                            } {session.ambr['downlink'] === undefined ?
                                "unlimited" :
                                  session.ambr.downlink['value'] ===
                                    undefined ?  "" :
                                  session.ambr.downlink['unit'] ===
                                    undefined ?  "bps" :
                                    session.ambr.downlink.unit === 0 ? "bps" :
                                    session.ambr.downlink.unit === 1 ? "Kbps" :
                                    session.ambr.downlink.unit === 2 ? "Mbps" :
                                    session.ambr.downlink.unit === 3 ? "Gbps" :
                                    session.ambr.downlink.unit === 4 ? "Tbps" :
                                        "Unknown Unit"
                            } / {session.ambr['uplink'] ===
                                    undefined ? "unlimited" :
                              session.ambr.uplink['value'] === undefined ?
                                "unlimited" : session.ambr.uplink.value
                            } {session.ambr['uplink'] === undefined ?
                                "unlimited" :
                                  session.ambr.uplink['value'] ===
                                    undefined ?  "" :
                                  session.ambr.uplink['unit'] ===
                                    undefined ?  "bps" :
                                    session.ambr.uplink.unit === 0 ? "bps" :
                                    session.ambr.uplink.unit === 1 ? "Kbps" :
                                    session.ambr.uplink.unit === 2 ? "Mbps" :
                                    session.ambr.uplink.unit === 3 ? "Gbps" :
                                    session.ambr.uplink.unit === 4 ? "Tbps" :
                                        "Unknown Unit"
                            }
                          </div>
                        }
                        <div className="large_data"></div>
                      </div>
                      {session['ue'] !== undefined &&
                        <div className="body">
                          <div className="medium_data"></div>
                          <div className="medium_data" style={{color:oc.gray[5]}}>{"UE IPv4"} </div>
                          <div className="large_data">{(session.ue || {}).addr}</div>
                          <div className="medium_data" style={{color:oc.gray[5]}}>{"UE IPv6"} </div>
                          <div className="large_data">{(session.ue || {}).addr6}</div>
                        </div>
                      }
                      {session['smf'] !== undefined &&
                        <div className="body">
                          <div className="medium_data"></div>
                          <div className="medium_data" style={{color:oc.gray[5]}}>{"SMF IPv4"} </div>
                          <div className="large_data">{(session.smf || {}).addr}</div>
                          <div className="medium_data" style={{color:oc.gray[5]}}>{"SMF IPv6"} </div>
                          <div className="large_data">{(session.smf || {}).addr6}</div>
                        </div>
                      }
                      {session['pcc_rule'] !== undefined &&
                        session.pcc_rule.map((pcc_rule, index) =>
                          <div key={index}>
                            <div className="body">
                              <div className="medium_data"></div>
                              <div className="medium_data"></div>
                              <div className="small_data">{pcc_rule.qos.index}</div>
                              <div className="small_data">{pcc_rule.qos.arp.priority_level}</div>
                              <div className="medium_data">
                                {pcc_rule.qos.arp.pre_emption_capability ===
                                    2 ? "Enabled" :
                                 pcc_rule.qos.arp.pre_emption_capability ===
                                    1 ? "Disabled" : "Unknown"}
                              </div>
                              <div className="medium_data">
                                {pcc_rule.qos.arp.pre_emption_vulnerability ===
                                    2 ? "Enabled" :
                                 pcc_rule.qos.arp.pre_emption_vulnerability ===
                                    1 ? "Disabled" : "Unknown"}
                              </div>
                              {pcc_rule.qos['mbr'] === undefined ?
                                <div className="large_data">
                                  unlimited/unlimited
                                </div> :
                                <div className="large_data">
                                  {pcc_rule.qos.mbr['downlink'] ===
                                      undefined ? "unlimited" :
                                    pcc_rule.qos.mbr.downlink['value'] ===
                                      undefined ? "unlimited" :
                                      pcc_rule.qos.mbr.downlink.value
                                  } {pcc_rule.qos.mbr['downlink'] ===
                                       undefined ? "unlimited" :
                                       pcc_rule.qos.mbr.downlink['value'] ===
                                          undefined ? "" :
                                       pcc_rule.qos.mbr.downlink['unit'] ===
                                          undefined ?  "bps" :
                                       pcc_rule.qos.mbr.downlink.unit === 0 ?
                                          "bps" :
                                       pcc_rule.qos.mbr.downlink.unit === 1 ?
                                          "Kbps" :
                                       pcc_rule.qos.mbr.downlink.unit === 2 ?
                                          "Mbps" :
                                       pcc_rule.qos.mbr.downlink.unit === 3 ?
                                          "Gbps" :
                                       pcc_rule.qos.mbr.downlink.unit === 4 ?
                                          "Tbps" : "Unknown Unit"
                                  } / {pcc_rule.qos.mbr['uplink'] ===
                                      undefined ? "unlimited" :
                                    pcc_rule.qos.mbr.uplink['value'] ===
                                      undefined ? "unlimited" :
                                      pcc_rule.qos.mbr.uplink.value
                                  } {pcc_rule.qos.mbr['uplink'] ===
                                       undefined ? "unlimited" :
                                       pcc_rule.qos.mbr.uplink['value'] ===
                                          undefined ?  "" :
                                       pcc_rule.qos.mbr.uplink['unit'] ===
                                          undefined ?  "bps" :
                                       pcc_rule.qos.mbr.uplink.unit === 0 ?
                                          "bps" :
                                       pcc_rule.qos.mbr.uplink.unit === 1 ?
                                          "Kbps" :
                                       pcc_rule.qos.mbr.uplink.unit === 2 ?
                                          "Mbps" :
                                       pcc_rule.qos.mbr.uplink.unit === 3 ?
                                          "Gbps" :
                                       pcc_rule.qos.mbr.uplink.unit === 4 ?
                                          "Tbps" : "Unknown Unit"
                                  }
                                </div>
                              }
                              {pcc_rule.qos['gbr'] === undefined ?
                                <div className="large_data">
                                  unlimited/unlimited
                                </div> :
                                <div className="large_data">
                                  {pcc_rule.qos.gbr['downlink'] ===
                                      undefined ? "unlimited" :
                                    pcc_rule.qos.gbr.downlink['value'] ===
                                      undefined ? "unlimited" :
                                      pcc_rule.qos.gbr.downlink.value
                                  } {pcc_rule.qos.gbr['downlink'] ===
                                       undefined ? "unlimited" :
                                       pcc_rule.qos.gbr.downlink['value'] ===
                                          undefined ?  "" :
                                       pcc_rule.qos.gbr.downlink['unit'] ===
                                          undefined ?  "bps" :
                                       pcc_rule.qos.gbr.downlink.unit === 0 ?
                                          "bps" :
                                       pcc_rule.qos.gbr.downlink.unit === 1 ?
                                          "Kbps" :
                                       pcc_rule.qos.gbr.downlink.unit === 2 ?
                                          "Mbps" :
                                       pcc_rule.qos.gbr.downlink.unit === 3 ?
                                          "Gbps" :
                                       pcc_rule.qos.gbr.downlink.unit === 4 ?
                                          "Tbps" : "Unknown Unit"
                                  } / {pcc_rule.qos.gbr['uplink'] ===
                                      undefined ? "unlimited" :
                                    pcc_rule.qos.gbr.uplink['value'] ===
                                      undefined ? "unlimited" :
                                      pcc_rule.qos.gbr.uplink.value
                                  } {pcc_rule.qos.gbr['uplink'] ===
                                       undefined ? "unlimited" :
                                       pcc_rule.qos.gbr.uplink['value'] ===
                                          undefined ?  "" :
                                       pcc_rule.qos.gbr.uplink['unit'] ===
                                          undefined ?  "bps" :
                                       pcc_rule.qos.gbr.uplink.unit === 0 ?
                                          "bps" :
                                       pcc_rule.qos.gbr.uplink.unit === 1 ?
                                          "Kbps" :
                                       pcc_rule.qos.gbr.uplink.unit === 2 ?
                                          "Mbps" :
                                       pcc_rule.qos.gbr.uplink.unit === 3 ?
                                          "Gbps" :
                                       pcc_rule.qos.gbr.uplink.unit === 4 ?
                                          "Tbps" : "Unknown Unit"
                                  }
                                </div>
                              }
                            </div>
                            {pcc_rule['flow'] !== undefined &&
                              pcc_rule.flow.map((flow, index) =>
                                <div className="body" key={index}>
                                  <div className="medium_data"></div>
                                  <div className="small_data" style={{color:oc.gray[5]}}>
                                    {flow.direction == 1 && "Downlink"}
                                    {flow.direction == 2 && "Uplink"}
                                  </div>
                                  <div className="large_data" style={{width:"480px"}}>{flow.description}</div>
                                </div>
                            )}
                          </div>
                      )}
                    </div>
                  )}
                </div>
              )}
            </Pdn>
          </Body>
        </Wrapper>
      </Modal>
      <Dimmed visible={visible}/>
    </div>
  )
}

export default View;
