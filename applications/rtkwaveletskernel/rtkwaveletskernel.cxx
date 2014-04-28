/*=========================================================================
 *
 *  Copyright RTK Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "rtkwaveletskernel_ggo.h"
#include "rtkGgoFunctions.h"

#include "rtkDaubechiesWaveletsKernelSource.h"
#include <itkImageFileWriter.h>

int main(int argc, char * argv[])
{
  GGO(rtkwaveletskernel, args_info);

  typedef float OutputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  // Create the kernel source
  typedef rtk::DaubechiesWaveletsKernelSource<OutputImageType> KernelSourceType;
  KernelSourceType::Pointer source = KernelSourceType::New();
  KernelSourceType::PassVector pass;
  for (unsigned int dim=0; dim<Dimension; dim++)
    {
    if(args_info.lowpass_arg[dim])
      {
      pass[dim] = KernelSourceType::Low;
      }
    else
      {
      pass[dim] = KernelSourceType::High;
      }
    }

  if (args_info.reconstruction_flag)
    {
    source->SetReconstruction();
    }
  else
    {
    source->SetDeconstruction();
    }
  source->SetOrder(args_info.order_arg);
  source->SetPass(pass);
  source->Update();

  // Write
  typedef itk::ImageFileWriter<  OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( args_info.output_arg );
  writer->SetInput( source->GetOutput() );
  TRY_AND_EXIT_ON_ITK_EXCEPTION( writer->Update() );

  return EXIT_SUCCESS;
}
