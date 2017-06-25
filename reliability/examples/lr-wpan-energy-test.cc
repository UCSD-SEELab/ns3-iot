/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Vishwesh Rege <vrege2012@gmail.com>
 */

#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/lr-wpan-module.h>
#include <ns3/simulator.h>
//#include "ns3/lr-wpan-radio-energy-model.h"
#include "ns3/basic-energy-source.h"

using namespace ns3;

void GetSetTRXStateConfirm (LrWpanPhyEnumeration status)
{
  std::cout << "At: " << Simulator::Now ()
                      << " Received Set TRX Confirm: " << status << "\n";
}

static void StateChangeNotification (std::string context, Time now, LrWpanPhyEnumeration oldState, LrWpanPhyEnumeration newState)
{
  std::cout << context << " state change at " << now.GetSeconds ()
                       << " from " << LrWpanHelper::LrWpanPhyEnumerationPrinter (oldState)
                       << " to " << LrWpanHelper::LrWpanPhyEnumerationPrinter (newState) << "\n";
}

static void GetTotalEnergyConsumption (std::string context, double oldValue, double newValue)
{
  std::cout << context << " TotalEnergyConsumption: " << newValue
                       << " from " << oldValue << "\n";
}

int main (int argc, char *argv[])
{
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("BasicEnergySource", LOG_LEVEL_ALL);

  LrWpanHelper lrWpanHelper;
  lrWpanHelper.EnableLogComponents ();

  Ptr<LrWpanPhy> phy0 = CreateObject<LrWpanPhy> ();

  Ptr<LrWpanRadioEnergyModel> em0 = CreateObject<LrWpanRadioEnergyModel> ();

  Ptr<BasicEnergySource> es0 = CreateObject<BasicEnergySource> ();
  es0->SetSupplyVoltage(3.3);

  em0->AttachPhy (phy0);
  em0->SetEnergySource (es0);
  es0->AppendDeviceEnergyModel (em0);

  es0->TraceConnect ("RemainingEnergy", std::string ("phy0"), MakeCallback (&GetTotalEnergyConsumption));

  phy0->TraceConnect ("TrxState", std::string ("phy0"), MakeCallback (&StateChangeNotification));

  phy0->SetPlmeSetTRXStateConfirmCallback (MakeCallback (&GetSetTRXStateConfirm));

  Simulator::Schedule (Seconds (1.0),&LrWpanPhy::PlmeSetTRXStateRequest,phy0,IEEE_802_15_4_PHY_TX_ON);
  Simulator::Schedule (Seconds (2.0),&LrWpanPhy::PlmeSetTRXStateRequest,phy0,IEEE_802_15_4_PHY_RX_ON);
  Simulator::Schedule (Seconds (3.0),&LrWpanPhy::PlmeSetTRXStateRequest,phy0,IEEE_802_15_4_PHY_TX_ON);
  Simulator::Schedule (Seconds (4.0),&LrWpanPhy::PlmeSetTRXStateRequest,phy0,IEEE_802_15_4_PHY_TRX_OFF);
  Simulator::Schedule (Seconds (5.0),&LrWpanPhy::PlmeSetTRXStateRequest,phy0,IEEE_802_15_4_PHY_RX_ON);
  Simulator::Schedule (Seconds (6.0),&LrWpanPhy::PlmeSetTRXStateRequest,phy0,IEEE_802_15_4_PHY_TRX_OFF);

  Simulator::Stop (Seconds (10.0));

  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
